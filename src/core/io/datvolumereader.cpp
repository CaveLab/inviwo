/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <inviwo/core/io/datvolumereader.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/stringconversion.h>

#include <iterator>

namespace inviwo {

DatVolumeReader::DatVolumeReader()
    : DataReaderType<VolumeVector>()
    , rawFile_("")
    , filePos_(0)
    , littleEndian_(true)
    , dimensions_(0)
    , format_(nullptr) {
    addExtension(FileExtension("dat", "Inviwo dat file format"));
}

DatVolumeReader::DatVolumeReader(const DatVolumeReader& rhs)
    : DataReaderType<VolumeVector>(rhs)
    , rawFile_(rhs.rawFile_)
    , filePos_(rhs.filePos_)
    , littleEndian_(rhs.littleEndian_)
    , dimensions_(rhs.dimensions_)
    , format_(rhs.format_){};

DatVolumeReader& DatVolumeReader::operator=(const DatVolumeReader& that) {
    if (this != &that) {
        rawFile_ = that.rawFile_;
        filePos_ = that.filePos_;
        littleEndian_ = that.littleEndian_;
        dimensions_ = that.dimensions_;
        format_ = that.format_;
        DataReaderType<VolumeVector>::operator=(that);
    }

    return *this;
}

DatVolumeReader* DatVolumeReader::clone() const { return new DatVolumeReader(*this); }

DatVolumeReader::VolumeVector* DatVolumeReader::readMetaData(std::string filePath) {
    if (!filesystem::fileExists(filePath)) {
        std::string newPath = filesystem::addBasePath(filePath);

        if (filesystem::fileExists(newPath)) {
            filePath = newPath;
        } else {
            throw DataReaderException("Error could not find input file: " + filePath, IvwContext);
        }
    }

    std::string fileDirectory = filesystem::getFileDirectory(filePath);
    std::string fileExtension = filesystem::getFileExtension(filePath);
    // Read the dat file content
    std::ifstream f(filePath.c_str());
    std::string textLine;
    std::string formatFlag = "";
    glm::mat3 basis(2.0f);
    glm::vec3 offset(0.0f);
    bool hasOffset = false;
    glm::vec3 spacing(0.0f);
    glm::mat4 wtm(1.0f);
    glm::vec3 a(0.0f), b(0.0f), c(0.0f);
    std::vector<std::string> parts;
    std::string key;
    std::string value;
    dvec2 datarange(0);
    dvec2 valuerange(0);
    std::string unit("");
    size_t sequences = 1;

    std::unordered_map<std::string, std::string> metadata;

    // For dat file containing references to multiple datfiles
    std::vector<std::string> datFiles;

    while (!f.eof()) {
        getline(f, textLine);

        textLine = trim(textLine);

        if (textLine == "" || textLine[0] == '#' || textLine[0] == '/') continue;
        parts = splitString(splitString(textLine, '#')[0], ':');
        if (parts.size() != 2) continue;

        key = toLower(trim(parts[0]));
        value = trim(parts[1]);

        std::stringstream ss(value);

        if (key == "objectfilename" || key == "rawfile") {
            rawFile_ = fileDirectory + value;
        } else if (key == "datfile") {
            datFiles.push_back(fileDirectory + value);
        } else if (key == "byteorder") {
            if (toLower(value) == "bigendian") {
                littleEndian_ = false;
            } else {
                littleEndian_ = true;
            }
        } else if (key == "sequences") {
            ss >> sequences;
        } else if (key == "resolution" || key == "dimensions") {
            ss >> dimensions_.x;
            ss >> dimensions_.y;
            ss >> dimensions_.z;
        } else if (key == "spacing" || key == "slicethickness") {
            ss >> spacing.x;
            ss >> spacing.y;
            ss >> spacing.z;
        } else if (key == "basisvector1") {
            ss >> a.x;
            ss >> a.y;
            ss >> a.z;
        } else if (key == "basisvector2") {
            ss >> b.x;
            ss >> b.y;
            ss >> b.z;
        } else if (key == "basisvector3") {
            ss >> c.x;
            ss >> c.y;
            ss >> c.z;
        } else if (key == "offset") {
            hasOffset = true;
            ss >> offset.x;
            ss >> offset.y;
            ss >> offset.z;
        } else if (key == "worldvector1") {
            ss >> wtm[0][0];
            ss >> wtm[1][0];
            ss >> wtm[2][0];
            ss >> wtm[3][0];
        } else if (key == "worldvector2") {
            ss >> wtm[0][1];
            ss >> wtm[1][1];
            ss >> wtm[2][1];
            ss >> wtm[3][1];
        } else if (key == "worldvector3") {
            ss >> wtm[0][2];
            ss >> wtm[1][2];
            ss >> wtm[2][2];
            ss >> wtm[3][2];
        } else if (key == "worldvector4") {
            ss >> wtm[0][3];
            ss >> wtm[1][3];
            ss >> wtm[2][3];
            ss >> wtm[3][3];
        } else if (key == "format") {
            ss >> formatFlag;
            // Backward support for USHORT_12 key
            if (formatFlag == "USHORT_12") {
                format_ = DataUINT16::get();
                // Check so that data range has not been set before
                if (glm::all(glm::equal(datarange, dvec2(0)))) {
                    datarange.y = 4095;
                }
            } else {
                format_ = DataFormatBase::get(formatFlag);
            }
        } else if (key == "datarange") {
            ss >> datarange.x;
            ss >> datarange.y;
        } else if (key == "valuerange") {
            ss >> valuerange.x;
            ss >> valuerange.y;
        } else if (key == "unit") {
            unit = value;
        } else {
            metadata[key] = value;
        }
    };

    // Check if other dat files where specified, and then only consider them as a sequence
    auto volumes = util::make_unique<VolumeVector>();

    if (!datFiles.empty()) {
        for (size_t t = 0; t < datFiles.size(); ++t) {
            auto datVolReader = util::make_unique<DatVolumeReader>();
            std::unique_ptr<VolumeVector> v(datVolReader->readMetaData(datFiles[t]));

            std::copy(std::make_move_iterator(v->begin()), std::make_move_iterator(v->end()),
                      std::back_inserter(*volumes));
        }
        LogInfo("Loaded multiple volumes: " << filePath << " volumes: " << datFiles.size());

    } else {
        if (dimensions_ == size3_t(0))
            throw DataReaderException(
                "Error: Unable to find \"Resolution\" tag in .dat file: " + filePath, IvwContext);
        else if (format_ == nullptr)
            throw DataReaderException(
                "Error: Unable to find \"Format\" tag in .dat file: " + filePath, IvwContext);
        else if (rawFile_ == "")
            throw DataReaderException(
                "Error: Unable to find \"ObjectFilename\" tag in .dat file: " + filePath,
                IvwContext);

        if (spacing != vec3(0.0f)) {
            basis[0][0] = dimensions_.x * spacing.x;
            basis[1][1] = dimensions_.y * spacing.y;
            basis[2][2] = dimensions_.z * spacing.z;
        }

        if (a != vec3(0.0f) && b != vec3(0.0f) && c != vec3(0.0f)) {
            basis[0][0] = a.x;
            basis[1][0] = a.y;
            basis[2][0] = a.z;
            basis[0][1] = b.x;
            basis[1][1] = b.y;
            basis[2][1] = b.z;
            basis[0][2] = c.x;
            basis[1][2] = c.y;
            basis[2][2] = c.z;
        }

        // If not specified, center the data around origo.
        if (!hasOffset) {
            offset = -0.5f * (basis[0] + basis[1] + basis[2]);
        }

        auto volume = std::make_shared<Volume>();
        volume->setBasis(basis);
        volume->setOffset(offset);
        volume->setWorldMatrix(wtm);
        volume->setDimensions(dimensions_);

        volume->dataMap_.initWithFormat(format_);
        if (datarange != dvec2(0)) {
            volume->dataMap_.dataRange = datarange;
        }
        if (valuerange != dvec2(0)) {
            volume->dataMap_.valueRange = valuerange;
        } else {
            volume->dataMap_.valueRange = volume->dataMap_.dataRange;
        }
        if (unit != "") {
            volume->dataMap_.valueUnit = unit;
        }

        volume->setDataFormat(format_);
        size_t bytes = dimensions_.x * dimensions_.y * dimensions_.z * (format_->getSize());

        for(auto elem : metadata) volume->setMetaData<StringMetaData>(elem.first, elem.second);

        for (size_t t = 0; t < sequences; ++t) {
            if (t==0) volumes->push_back(std::move(volume));
            else volumes->push_back(std::shared_ptr<Volume>(volumes->front()->clone()));
            auto diskRepr = util::make_unique<VolumeDisk>(filePath, dimensions_, format_);
            filePos_ = t * bytes;
            diskRepr->setDataReader(this->clone());
            volumes->back()->addRepresentation(diskRepr.release());
        }

        std::string size = formatBytesToString(bytes * sequences);
        LogInfo("Loaded volume sequence: " << filePath << " size: " << size);
    }
    return volumes.release();
}

void DatVolumeReader::readDataInto(void* destination) const {
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);

    if (fin.good()) {
        std::size_t size = dimensions_.x * dimensions_.y * dimensions_.z * (format_->getSize());
        fin.seekg(filePos_);
        fin.read(static_cast<char*>(destination), size);

        if (!littleEndian_ && format_->getSize() > 1) {
            std::size_t bytes = format_->getSize();
            char* temp = new char[bytes];

            for (std::size_t i = 0; i < size; i += bytes) {
                for (std::size_t j = 0; j < bytes; j++)
                    temp[j] = static_cast<char*>(destination)[i + j];

                for (std::size_t j = 0; j < bytes; j++)
                    static_cast<char*>(destination)[i + j] = temp[bytes - j - 1];
            }

            delete[] temp;
        }
    } else
        throw DataReaderException("Error: Could not read from raw file: " + rawFile_, IvwContext);

    fin.close();
}

void* DatVolumeReader::readData() const {
    std::size_t size = dimensions_.x * dimensions_.y * dimensions_.z * (format_->getSize());
    char* data = new char[size];

    if (data) {
        readDataInto(data);
    } else {
        throw DataReaderException(
            "Error: Could not allocate memory for loading raw file: " + rawFile_, IvwContext);
    }

    return data;
}

}  // namespace

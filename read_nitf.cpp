/* =========================================================================
 * This file is part of NITRO
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * NITRO is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */

// modeled after nitf_extract.py
// and
// */nitro-NITRO-2.11.2/modules/c++/nitf/tests/test_extract.cpp`

#include <stdio.h>
#include <iostream>

#include <string>
#include <vector>
#include <numeric> // std::iota

#include <import/str.h>
#include <import/sys.h>

#include <import/nitf.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

static  std::vector<std::string> extract_image(const nitf::ImageSubheader& subheader, uint32_t index, nitf::ImageReader& imageReader,
    const std::string& outDir="", const std::string& baseName="")
{
    cout << "inside extract_image" << endl;
    nitf::SubWindow window(subheader);
    const auto nbpp = subheader.actualBitsPerPixel(); //const auto nbpp = subheader.numBitsPerPixel();
    cout << "num bytes per pixel " << nbpp << endl;
    const auto bandData = imageReader.read(window, nbpp);
    cout << "wad bandData read?" << endl;

    std::vector<std::string> outNames;
    cout << "outNames" <<outNames[0] << endl;
    size_t band = 0;
    for (const auto& data : bandData) // for band, data in enumerate(bandData):
    {
        auto outName = str::format("%s_%d__%d_x_%d_%d_band_%d.out",
            baseName.c_str(), index, window.getNumRows(), window.getNumCols(), static_cast<int>(nbpp), static_cast<int>(band));
        outName = sys::Path::joinPaths(outDir, outName);
        auto f = fopen(outName.c_str(), "wb"); // f = open(outName, "wb");
        nitf::write(f, data); // fwrite(data.data(), sizeof(data[0]), data.size() / sizeof(data[0]), f);
        fclose(f); // f.close();
        outNames.push_back(outName);
        band++;
    }

    return outNames;
}

static void extract_images(const std::string& fileName, const std::string& outDir = "")
{
    cout << "inside extract images" << endl;
    nitf::IOHandle handle(fileName);
    nitf::Reader reader;
    nitf::Record record = reader.read(handle);

    uint32_t i = 0;
    for (nitf::ImageSegment segment : record.getImages())
    {
        auto imReader = reader.newImageReader(i);
        extract_image(segment.getSubheader(), i, imReader, outDir, sys::Path::basename(fileName));

        i++;
    }

    handle.close();
}

int main(int argc, char **argv)
{
    cout << "starting program..." << "\n";

    string filepath = "/home/dennis/Downloads/WPAFB-21Oct2009-TRAIN_NITF_003/WPAFB-21Oct2009/Data/TRAIN/NITF/";
    filepath += "20091021203201-01000605-VIS.ntf.r0";

    auto ntf_ver = nitf::Reader::getNITFVersion(filepath);
    if (IS_NITF20(ntf_ver)){
        cout << "NITF v2.0" << "\n";
    } else if(IS_NITF21(ntf_ver)){
        cout << "NITF v2.1" << "\n";
    } else {
        cerr << "UNKNOWN VERSION" << endl;
        return -1;
    }

    cout << "opening file" << "\n";
    nitf::IOHandle handle(filepath);

    nitf::Reader reader;
    nitf::Record record = reader.read(handle);
    nitf::FileHeader header = record.getHeader();

    // read header fields
    string FHDR = header.fileHeader();
    string FVER = header.fileVersion();
    nitf::Field CLEVEL = header.getComplianceLevel();
    nitf::Field STYPE = header.getSystemType();
    nitf::Field OSTAID = header.getOriginStationID();
    nitf::Field FDT = header.getFileDateTime();
    nitf::Field FTITLE = header.getFileTitle();
    string FSCLAS = header.getClassification();

    nitf::FileSecurity file_security = header.getSecurityGroup();
    string FSCLSY = file_security.getClassificationSystem();
    string FSCODE = file_security.getCodewords();
    nitf::Field FSCTLH = file_security.getControlAndHandling();
    nitf::Field FSREL = file_security.getReleasingInstructions();
    nitf::Field FL = header.getFileLength();
    nitf::Field HL = header.getHeaderLength();
    nitf::Field NUMI = header.getNumImages();
    nitf::Field NUMS = header.getNumGraphics();

    int image_number = 0;
    nitf::ComponentInfo image_info = header.getImageInfo(image_number);
    nitf::Field LISH001 = image_info.getLengthSubheader();
    nitf::Field LI001 = image_info.getLengthData();

    

    cout << FHDR << "\n";
    cout << FVER << "\n";
    cout << CLEVEL.toString() << "\n";
    cout << STYPE.toString() << "\n";
    cout << OSTAID.toString() << "\n";
    cout << FDT.toString() << "\n";
    cout << FTITLE.toString() << "\n";
    cout << FSCLAS << "\n";
    cout << FSCLSY << "\n";
    cout << FSCODE << "\n";
    cout << FSCTLH.toString() << "\n";
    cout << FSREL.toString() << "\n";
    cout << FL.toString() << "\n";
    cout << HL.toString() << "\n";
    cout << NUMI.toString() << "\n";
    cout << NUMS.toString() << "\n";
    cout << LISH001.toString() << "\n";
    cout << LI001.toString() << "\n";

    cout << "closing file" << "\n";
    handle.close();

    cout << "finished." << endl;


 
}


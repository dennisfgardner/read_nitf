/* Read NITF

The NITRO library by mdaus is used https://github.com/mdaus/nitro which is
under the GNU Lesser General Public License v3.0.

 */

#include <iostream>
#include <string>
#include <filesystem>

#include "import/nitf.hpp"
#include "opencv2/core.hpp"
#include <opencv2/imgcodecs.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

#define PRINT(X) cout << "\t " << #X << " = " << X.toString() << "\n"


void get_and_print_header_fields(nitf::FileHeader &header){
    /*TODO clean this up, use nitf::Field for all to use PRINT macro
    also think about using a map to loop through keys to print
    and comment about field names obtained from doc
    pull out file security into it's own function*/
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
    PRINT(CLEVEL);
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
    PRINT(LI001);
}


void get_and_print_segment_header_fields(nitf::ImageSubheader &seg_header){
    nitf::Field IM = seg_header.getFilePartType();
    nitf::Field IID1 = seg_header.getImageId();
    nitf::Field IDATIM = seg_header.getImageDateAndTime();
    nitf::Field TGTID = seg_header.getTargetId();
    nitf::Field IID2 = seg_header.getImageTitle();
    nitf::Field ISCLAS = seg_header.getImageSecurityClass();

    nitf::FileSecurity img_security = seg_header.getSecurityGroup();
    nitf::Field ISCLSY = img_security.getClassificationSystem();
    nitf::Field ISCODE = img_security.getCodewords();
    nitf::Field ISCTLH = img_security.getControlAndHandling();
    nitf::Field ISREL = img_security.getReleasingInstructions();

    nitf::Field ENCRYP = seg_header.getEncrypted();
    nitf::Field ISORCE = seg_header.getImageSource();
    nitf::Field NROWS = seg_header.getNumRows();
    nitf::Field NCOLS = seg_header.getNumCols();
    nitf::Field PVTYPE = seg_header.getPixelValueType();
    nitf::Field IREP = seg_header.getImageRepresentation();
    nitf::Field ICAT = seg_header.getImageCategory();
    nitf::Field ABPP = seg_header.getActualBitsPerPixel();

    PRINT(IM);
    PRINT(IID1);
    PRINT(IDATIM);
    PRINT(TGTID);
    PRINT(IID2);
    PRINT(ISCLAS);
    PRINT(ISCLSY);
    PRINT(ISCODE);
    PRINT(ISCTLH);
    PRINT(ISREL);
    PRINT(ENCRYP);
    PRINT(ISORCE);
    PRINT(NROWS);
    PRINT(NCOLS);
    PRINT(PVTYPE);
    PRINT(IREP);
    PRINT(ICAT);
    PRINT(ABPP);

    nitf::Field PJUST = seg_header.getPixelJustification();
    nitf::Field ICORDS = seg_header.getImageCoordinateSystem();
    nitf::Field IGEOLO = seg_header.getCornerCoordinates();
    nitf::Field NICOM = seg_header.getNumImageComments();
    nitf::Field IC = seg_header.getImageCompression();
    nitf::Field COMRAT = seg_header.getCompressionRate();
    nitf::Field NBANDS = seg_header.getNumImageBands();
    nitf::Field XBANDS = seg_header.getNumMultispectralImageBands();

    // TODO this should be a loop if there are multiple bands
    nitf::BandInfo band_info = seg_header.getBandInfo(0);
    nitf::Field IREPBAND1 = band_info.getRepresentation();
    nitf::Field ISUBCAT = band_info.getSubcategory();

    nitf::Field ISYNC = seg_header.getImageSyncCode();
    nitf::Field IMODE = seg_header.getImageMode();
    nitf::Field NBPR = seg_header.getNumBlocksPerRow();
    nitf::Field NBPC = seg_header.getNumBlocksPerCol();
    nitf::Field NPPBH = seg_header.getNumPixelsPerHorizBlock();
    nitf::Field NPPBV = seg_header.getNumPixelsPerVertBlock();
    nitf::Field NBPP = seg_header.getNumBitsPerPixel();

    PRINT(PJUST);
    PRINT(ICORDS);
    PRINT(IGEOLO);
    PRINT(NICOM);
    PRINT(IC);
    PRINT(COMRAT);
    PRINT(NBANDS);
    PRINT(XBANDS);
    PRINT(IREPBAND1);
    PRINT(ISUBCAT);
    PRINT(ISYNC);
    PRINT(IMODE);
    PRINT(NBPR);
    PRINT(NBPC);
    PRINT(NPPBH);
    PRINT(NPPBV);
    PRINT(NBPP);
}

int main(int argc, char **argv)
{
    cout << "starting NITF reading program..." << "\n";

    // check number of args
    if (argc != 2){
        cerr << "ERROR: INCORRECT NUMBER OF ARGUMENTS" << "\n";
        cerr << "usage: ./read_nitf <path_to_nitf>" <<endl;
        return -1;
    }

    // get and check filepath
    string filepath = argv[1];
    if (!std::filesystem::exists(filepath)){
        cerr << filepath << "\n";
        cerr << "ERROR: FILE DOES NOT EXIST" << endl;
        return -1;
    }
    
    // get and check NITF version
    cout << "checking NITF version" << "/n";
    nitf::Version ver = nitf::Reader::getNITFVersion(filepath);
    if (IS_NITF20(ver)){
        cout << "\t NITF v2.0" << "\n";
    } else if(IS_NITF21(ver)){
        cout << "\t NITF v2.1" << "\n";
    } else {
        cerr << "ERROR: UNKNOWN NITF VERSION" << endl;
        return -1;
    }

    cout << "opening file" << "\n";
    nitf::IOHandle handle(filepath);
    nitf::Reader reader;
    nitf::Record record = reader.read(handle);

    // get and print header fields
    nitf::FileHeader header = record.getHeader();
    get_and_print_header_fields(header);


    nitf::List images = record.getImages();
    cout << "number of images " << images.getSize() << "\n";

    // TODO loop through image segments
    int segment_number = 0;
    nitf::ImageSegment segment = images[segment_number];
    nitf::ImageSubheader seg_header = segment.getSubheader();
    get_and_print_segment_header_fields(seg_header);
    
    nitf::ImageReader img_reader = reader.newImageReader(segment_number);
    nitf::SubWindow window(seg_header);
    nitf::Field NROWS = seg_header.getNumRows();
    nitf::Field NCOLS = seg_header.getNumCols();
    nitf::Field NBPP = seg_header.getNumBitsPerPixel();
    nitf::BufferList<std::byte> band_data = img_reader.read(window, NBPP);

    int rows = (int)NROWS;
    int cols = (int)NCOLS;
    cout << "rows " << rows << " cols " << cols << "\n";
    cout << cv::getVersionMajor() << cv::getVersionMinor << cv::getVersionRevision << "\n";
    cv::Mat img = cv::Mat(rows, cols, CV_8UC1, band_data[0]);
    // cv::imwrite("out.png", img);

    // cout << "band_data" << band_data[0] << endl;

    cout << "closing file" << "\n";
    handle.close();

    cout << "finished." << endl;


 
}


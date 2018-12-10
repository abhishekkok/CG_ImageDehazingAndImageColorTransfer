#include "ImageProcessingService.h"
#include "SegmentationBasedImageDehazing.h"
#include "ImageDehazing.h"
#include "GamutImageColorConvolution.h"
#include "ImageConvolution.h"
using namespace std;


void ImageProcessingService::ImageDehazingTechnique(int index,string fileName)
{	
	cout << "Inside with " << index << " with fileName " << fileName <<" \n";
	if (index == 1)
	{
		ImageDehazing dehazer;
		dehazer.loadImage(fileName);
		dehazer.dehazeImage(0, 0.01, 0.95);
		dehazer.writeImage("dehazerFileResult.jpg");
		dehazer.display();
	}
	else if (index == 2) {
		SegmentationBasedImageDehazing temp = SegmentationBasedImageDehazing();
		temp.performImageSegmentationBasedDehazing(fileName);
	}

	cout << "Finished with dehazing " << index << " for fileName " << fileName << " \n";
	cout << "*********************************************************************** \n";

}


void ImageProcessingService::ImageColorTransferTechnique(int index , string sourceFile , string targetFile)
{
	cout << " inside with " << index << " with sourceFile " << sourceFile << " and targetFile " << targetFile << " \n";
	
	if (index == 1)
	{
		ImageConvolution ic;
		Mat finalImage = ic.getConvolutedImage(sourceFile, targetFile);
		string outputFile = "colorTransferOutput.png";
		ic.writeImage(outputFile, finalImage);
		ic.displayAll(sourceFile, targetFile, outputFile);

	}
	else if (index == 2)
	{
		GamutImageColorConvolution gamutCC;
		Mat finalResult = gamutCC.gamutImageColorTransfer(sourceFile, targetFile);
		string outputFile = "colorTransferOutput.jpg";
		string whiteBalanceFile = "whiteBalancedTarget.jpg";
		imwrite(outputFile, finalResult);
		gamutCC.displayAll(sourceFile, targetFile, outputFile, whiteBalanceFile);
	}

	cout << "Finished with " << index << " for sourceFile " << sourceFile << " and targetFile " << targetFile << " \n";
	cout << "*********************************************************************** \n";
}

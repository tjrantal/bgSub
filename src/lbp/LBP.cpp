#ifndef _LBP_H
	#include "LBP.h"
#endif
#include <cmath>
//Constructors
LBP::LBP(int samples, int radius, int threshold){
	this->samples=samples;
	this->radius=radius;
	this->threshold=threshold;
	cutPoints = new double[samples+2];
	for (int i = 0; i<samples+2;++i){
		cutPoints[i] = i;
	}
	mapping = getMapping(samples);
	neighbourhood = getCircularNeighbourhood(radius,samples);
}
	
LBP::LBP(int samples, int radius): LBP(samples,radius,3){};
LBP::LBP(): LBP(8,1){};

//Destructor;
LBP::~LBP(){

}


/*Mapping to rotation invariant uniform patterns: riu2 in getmapping.m*/	
Mapping* LBP::getMapping(int samples){
	int bitMaskLength = (int) (std::pow(2.0,(double) samples));
	char* table = new char[bitMaskLength];
	int j;
	int sampleBitMask = 0;
	for (int i = 0;i<samples;++i){
		sampleBitMask |= 1<<i;
	}
	
	int numt;
	for (int i = 0;i<bitMaskLength;++i){
		j = ((i<<1) & sampleBitMask); //j = bitset(bitshift(i,1,samples),1,bitget(i,samples)); %rotate left
		j = (i>>(samples-1)) >0 ? j | 1: j & ~1;	//Set first bit to one or zero
		numt = 0;
		for (int k = 0;k<samples;++k){
			numt+= (((i^j)>>k) & 1);
		}
		
		if (numt <= 2){
			for (int k = 0;k<samples;++k){
				table[i]+= (i>>k) & 1;
			}
		}else{
			table[i] = (char) (samples+1);
		}
	}
	return new Mapping(table,bitMaskLength);
}
	
/*Neighbourhood coordinates relative to pixel of interest*/
Neighbourhood* LBP::getCircularNeighbourhood(int radius,int samples){
	double PI = std::acos(-1);
	double** samplingCoordinates = new double*[samples];
	double angleIncrement = PI*2.0/(double) samples;
	for (int n = 0;n <samples; ++n){
		samplingCoordinates[n] = new double[2];
		samplingCoordinates[n][0] = ((double) radius)*std::cos(((double)n)*angleIncrement);
		samplingCoordinates[n][1] = ((double) radius)*std::sin(((double)n)*angleIncrement);
	}
	return new Neighbourhood(samplingCoordinates,samples);
}

char** LBP::getLBP(double** data, int width, int height){
	this->width = width;
	this->height = height;
	char** lbpSlice = new char*[width];
	for (int i = 0;i<width;++i){
		lbpSlice[i] = new char[height];
	}
	
	
	/*Calculate the lbp*/
	int* coordinates = new int[2];
	for (int i = 0+radius;i<width-radius;++i){
		for (int j = 0+radius;j<height-radius;++j){
			coordinates[0] = i;
			coordinates[1] = j;
			//System.out.println("source x "+coordinates[0]+" y "+coordinates[1]);
			lbpSlice[i][j] = lbpBlock(data,coordinates);
		}
	}
	
	return lbpSlice;
}

char LBP::lbpBlock(double** data,int* coordinates){
	int lbpValue = 0;
	double x = (double) coordinates[0];
	double y = (double) coordinates[1];
	for (int i = 0; i<neighbourhood->length;++i){
		lbpValue = data[coordinates[0]][coordinates[1]] > getBicubicInterpolatedPixel(x+neighbourhood->coordinates[i][0],y+neighbourhood->coordinates[i][1],data)+threshold ? lbpValue | (1 << i) : lbpValue & ~(1 << i);
	}
	return mapping->map[lbpValue];
}


/** This method is from Chapter 16 of "Digital Image Processing:
		An Algorithmic Introduction Using Java" by Burger and Burge
		(http://www.imagingbook.com/). */
double LBP::getBicubicInterpolatedPixel(double x0, double y0, double** data) {
	int u0 = (int) std::floor(x0);	//use floor to handle negative coordinates too
	int v0 = (int) std::floor(y0);
	if (u0<1 || u0>width-3 || v0< 1 || v0>height-3){
		/*If cannot do bicubic use bilinear interpolation http://en.wikipedia.org/wiki/Bilinear_interpolation*/
		if ((u0 == 0 || u0 < width-1) && (v0 == 0 || v0 < height-1)){ 
			double x = (x0-(double)u0);
			double y = (y0-(double)v0);
			return data[u0][v0]*(1-x)*(1-y) 	/*f(0,0)(1-x)(1-y)*/
					+data[u0+1][v0]*(1-y)*x	/*f(1,0)x(1-y)*/
					+data[u0][v0+1]*(1-x)*y	/*f(0,1)(1-x)y*/
					+data[u0+1][v0+1]*x*y;	/*f(1,1)xy*/
		}
		return 0; //Return zero for points outside the interpolable area
	}else{
		//Bicubic interpolation
		double q = 0;
		for (int j = 0; j < 4; ++j) {
			int v = v0 - 1 + j;
			double p = 0;
			for (int i = 0; i < 4; ++i) {
				int u = u0 - 1 + i;
				p = p + data[u][v] * cubic(x0 - u);
			}
			q = q + p * cubic(y0 - v);
		}
		return q;
	}
}
	
	
double LBP::cubic(double x) {
	double a = 0.5; // Catmull-Rom interpolation
	if (x < 0.0) x = -x;
	double z = 0.0;
	if (x < 1.0){ 
		z = x*x*(x*(-a+2.0) + (a-3.0)) + 1.0;
	}else{
	 	if (x < 2.0){ 
			z = -a*x*x*x + 5.0*a*x*x - 8.0*a*x + 4.0*a;
		}
	}
	return z;
}

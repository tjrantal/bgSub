/*A class for calculating rotation invariant uniform local binary pattern in
	circular neighbourhood of up to 127 neighbours and with integer values radius.


Bicubic interpolation copied and ported from imageJ imageProcessor.


	LBP modified from Oulu machine vision group lbp code (http://www.cse.oulu.fi/CMV/Downloads).
	
	tjrantal at gmail dot com 2016
*/
#ifndef _LBP_H
#define _LBP_H

struct Neighbourhood{
	int length;
	double** coordinates;
	Neighbourhood(double** coordinates, int length){
		this->coordinates = coordinates;
		this->length = length;
	}
	~Neighbourhood(){
		for (int i = 0; i<length;++i){
			delete[] coordinates[i];
		}
		delete[] coordinates;
	}
	
};

struct Mapping{
	int length;
	char* map;
	Mapping(char* map, int length){
		this->map=map;
		this->length=length;
	}
	~Mapping(){
		delete[] map;
	}
};


class LBP{
	private:
		int width;
		int height;
		
		double* cutPoints;
		//Methods
		char lbpBlock(double** data,int* coordinates);

		/*Mapping to rotation invariant uniform patterns: riu2 in getmapping.m*/	
		Mapping* getMapping(int samples);

	
		/*Neighbourhood coordinates relative to pixel of interest*/
		Neighbourhood* getCircularNeighbourhood(int radius,int samples);
		
		/*Cubic interpolation from ImageJ*/
		double getBicubicInterpolatedPixel(double x0, double y0, double** data);
		double cubic(double x);
	
	public:
		int radius;
		int samples;
		int threshold;
		Mapping *mapping;
		Neighbourhood *neighbourhood;
		
		
		/*Constructors*/
		LBP(int samples, int radius, int threshold);	
		LBP(int samples, int radius);
		LBP();
		
		/**Destructor*/
		~LBP();
		
		/*Return the LBP image*/
		char** getLBP(double** data, int width, int height);

		
};
#endif

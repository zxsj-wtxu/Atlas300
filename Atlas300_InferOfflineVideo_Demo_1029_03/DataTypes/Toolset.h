/*
 * Toolset.h
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#ifndef TOOLSET_H_
#define TOOLSET_H_
#include "CalibrationInfo.h"
extern"C"{
#include <iconv.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <ft2build.h>
#include <math.h>
#include FT_FREETYPE_H
}
typedef struct OsdImage {
	char** pimage;
	int width;
	int height;
	int osd_size;
	int x;
	int y;
	int font;
	OsdImage(){
		pimage = NULL;
		width = 0;
		height = 0;
		osd_size = 0;
		x = 0;
		y = 0;
		font = 36;
	};
	OsdImage(int _x, int _y, int _osd_size, int _font){
		pimage = NULL;
		width = 0;
		height = 0;
		osd_size = _osd_size;
		x = _x;
		y = _y;
		font = _font;
	};
	OsdImage(char** _pimage, int _x, int _y, int _width, int _height, int _osd_size, int _font){
		pimage = _pimage;
		width = _width;
		height = _height;
		osd_size = _osd_size;
		x = _x;
		y = _y;
		font = _font;
	};
}OSDIMAGE;

class Toolset {
public:
	virtual ~Toolset();

	static Toolset* Instance();
	static void Destroy();

	double RectOverlap(cv::Rect& r, cv::Rect& rOther);

	bool toImageFile(const MyString& deviceid, const MyString& areatype, const MyString& filepath);
	int clear(const MyString& deviceid, const MyString& areatype);
	int removeArea(const MyString& deviceid, const MyString& areatype, int value);
	int AreaValue(const MyString& deviceid, const MyString& areatype, int x, int y);
	int addArea(const MyString& deviceid, const MyString& areatype, int value, const vector<cv::Point>& points);
	int updateArea(const MyString& deviceid, const MyString& areatype, int value, const vector<cv::Point>& points);
	int registCalibrationInfo(const MyString& deviceid, const MyString& areatype, int width = 1920, int height = 1080);

	int ConvertCharToImage(char* charText, OSDIMAGE& _osdimage, int size);
	int ConvertCharToImage(char* charText, const size_t charlen, OSDIMAGE& _osdimage, int size=36);
	int ConvertWcharToImage(wchar_t* wcharText, const size_t wcharlen, OSDIMAGE& _osdimage, int size=36);
	int ConvertTimeToImage(OSDIMAGE& _osdimage, int size=36);
	int ConvertTimeToImage(char* charText, OSDIMAGE& _osdimage, int size);
	bool ReleaseOsdImage(OSDIMAGE& _osdimage);
	bool InitOsdImage(OSDIMAGE& _osdimage, char value);
	bool AllocOsdImage(OSDIMAGE& _osdimage);
	void GetNowTime(char* timE);
private:
	Toolset();
	static Toolset* ins_toolset;
	static pthread_mutex_t mutex_ins_toolset;

	map<const MyString, CalibrationInfo*> devices_calinfo;
	pthread_mutex_t mutex_images = PTHREAD_MUTEX_INITIALIZER;

	string configfile = "./simsun.ttc";
	bool utf8_to_unicode(char *inbuf, size_t *inlen, wchar_t **w_out, size_t *w_outlen);

	int Get_FT_BitMap(char**image,char* charText,int u32TextSize,int u32Height,int u32Width);
	int Get_FT_Wchar_BitMap(char **image, wchar_t* wcharText,int u32TextSize,int u32Height,int u32Width);
	int Get_FT_Wchar_BitMap(char **image, wchar_t* wcharText,int u32TextSize,int u32Height,int u32Width, int u32Wsize);
};

#endif /* TOOLSET_H_ */

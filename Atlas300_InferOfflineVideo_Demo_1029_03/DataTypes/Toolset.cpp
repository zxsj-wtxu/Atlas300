/*
 * Toolset.cpp
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#include "Toolset.h"
Toolset* Toolset::ins_toolset = NULL;
pthread_mutex_t Toolset::mutex_ins_toolset = PTHREAD_MUTEX_INITIALIZER;

Toolset::Toolset(){

}

Toolset::~Toolset(){
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		for(it=devices_calinfo.begin(); it != devices_calinfo.end();){
			if(it->second != NULL)
				delete it->second;
			it = devices_calinfo.erase(it);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	cout << "~Toolset" <<endl;
}

Toolset* Toolset::Instance(){
	if(pthread_mutex_lock(&mutex_ins_toolset) == 0){
		if(ins_toolset == NULL){
			ins_toolset = new Toolset;
		}
		pthread_mutex_unlock(&mutex_ins_toolset);
	}
	return ins_toolset;
}

void Toolset::Destroy(){
	if(pthread_mutex_lock(&mutex_ins_toolset) == 0){
		if(ins_toolset != NULL){
			cout << "Toolset::Destroy" <<endl;
			delete ins_toolset;
		}
		pthread_mutex_unlock(&mutex_ins_toolset);
	}
}

double Toolset::RectOverlap(cv::Rect& r, cv::Rect& rOther)
{
	int x0 = std::max(r.x , rOther.x);
	int x1 = std::min(r.x + r.width, rOther.x + rOther.width);
	int y0 = std::max(r.y, rOther.y);
	int y1 = std::min(r.y + r.height, rOther.y + rOther.height);

	if (x0 >= x1 || y0 >= y1) return 0.f;

	double areaInt = (x1-x0)*(y1-y0);
	return areaInt/((double)r.width*r.height+(double)rOther.width*rOther.height-areaInt);
}

bool Toolset::toImageFile(const MyString& deviceid, const MyString& areatype, const MyString& filepath){
	bool ret = false;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = false;
		}else{
			ret = it->second->toFile(areatype, filepath);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::clear(const MyString& deviceid, const MyString& areatype){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->clear(areatype);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::removeArea(const MyString& deviceid, const MyString& areatype, int value){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->removeArea(areatype, value);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::AreaValue(const MyString& deviceid, const MyString& areatype, int x, int y){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
//			printf("cannot find %s deviceid.\n", deviceid.c_str());
		}else{
			ret = it->second->AreaValue(areatype, x,y);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::addArea(const MyString& deviceid, const MyString& areatype, int value, const vector<cv::Point>& points){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->addArea(areatype, value, points);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::updateArea(const MyString& deviceid, const MyString& areatype, int value, const vector<cv::Point>& points){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->updateArea(areatype, value, points);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::registCalibrationInfo(const MyString& deviceid, const MyString& areatype, int width, int height){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::const_iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			devices_calinfo.insert(map<const MyString, CalibrationInfo*>::value_type(deviceid, new CalibrationInfo(deviceid, areatype, width, height)));
			ret = 0;
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	cout << "registCalibrationInfo done" <<endl;
	return ret;
}

bool Toolset::AllocOsdImage(OSDIMAGE& _osdimage){
	int temp = 0;
	int width = _osdimage.width;
	int height = _osdimage.height;

	if(!(height > 0 && width > 0)) return false;

	if(_osdimage.pimage != NULL) return true;

	_osdimage.pimage = (char**)calloc(height, sizeof(char*));
	if(_osdimage.pimage == NULL){
		return false;
	}
	for(int i=0; i<height; i++){
		temp = i;
		_osdimage.pimage[i] = (char*)calloc(width, sizeof(char));
		if(_osdimage.pimage[i] == NULL){
			for(temp; temp > 0; temp--){
				printf("free _osdimage.pimage[%d], line:%d\n", i, __LINE__);
				free(_osdimage.pimage[i]);
			}
			if(_osdimage.pimage != NULL){
				printf("free _osdimage.pimage, line:%d\n", __LINE__);
				free(_osdimage.pimage);
			}
			return false;
		}
	}
	return true;
}
/*
 * 初始化图片
 * */
bool Toolset::InitOsdImage(OSDIMAGE& _osdimage, char value){
	int width = _osdimage.width;
	int height = _osdimage.height;

	if(!(height > 0 && width > 0)) return false;

	if(_osdimage.pimage == NULL) return false;

	for(int i=0; i<height; i++){
		if(_osdimage.pimage[i] == NULL) return false;
		memset(_osdimage.pimage[i], value, sizeof(char)*width);
	}
	return true;
}
/*
 * 释放指定的OSD图片
 * */
bool Toolset::ReleaseOsdImage(OSDIMAGE& _osdimage){
	int width = _osdimage.width;
	int height = _osdimage.height;

    for(int i=0; i<_osdimage.height; i++){
    	if(_osdimage.pimage[i] != NULL)
    		free(_osdimage.pimage[i]);
    }
    if(_osdimage.pimage != NULL)
    	free(_osdimage.pimage);
	return true;
}

int Toolset::ConvertCharToImage(char* charText, OSDIMAGE& _osdimage, int size){
	_osdimage.width = size / 2 * strlen(charText);
	_osdimage.height= size;

	if(!AllocOsdImage(_osdimage)){
		printf("AllocOsdImage failed.\n");
		return -1;
	}

	if(!InitOsdImage(_osdimage, 0)) {
		printf("InitOsdImage failed.\n");
		return -1;
	}
	//char字符为wchar_t一半，20*（36/2）=360
	return Get_FT_BitMap(_osdimage.pimage, charText, size, _osdimage.height, _osdimage.width);
}

int Toolset::ConvertCharToImage(char* charText, const size_t charlen, OSDIMAGE& _osdimage, int size){
	_osdimage.width = size / 2 * strlen(charText);
	_osdimage.height= size;

	if(!AllocOsdImage(_osdimage)){
		printf("AllocOsdImage failed.\n");
		return -1;
	}

	if(!InitOsdImage(_osdimage, 0)) {
		printf("InitOsdImage failed.\n");
		return -1;
	}
	//char字符为wchar_t一半，20*（36/2）=360
	return Get_FT_BitMap(_osdimage.pimage, charText, size, _osdimage.height, _osdimage.width);

//	size_t inlen = charlen;
//	wchar_t* wcharText = NULL;
//	size_t wcharlen = 0;
//
//	printf("utf8_to_unicode\n");
//	utf8_to_unicode(charText, &inlen, &wcharText, &wcharlen);
//	printf("utf8_to_unicode done\n");
//
//	int ret = ConvertWcharToImage(wcharText, (const size_t)wcharlen, _osdimage, size);
//	printf("ConvertWcharToImage done\n");
//
//    return ret;
}

int Toolset::ConvertWcharToImage(wchar_t* wcharText, const size_t wcharlen, OSDIMAGE& _osdimage, int size){
	_osdimage.width = size * wcharlen;
	_osdimage.height= size;

	printf("AllocOsdImage");
	if(!AllocOsdImage(_osdimage)){
		printf("AllocOsdImage failed.\n");
		return -1;
	}
	if(!InitOsdImage(_osdimage, 0)) {
		printf("InitOsdImage failed.\n");
		return -1;
	}

    return Get_FT_Wchar_BitMap(_osdimage.pimage, wcharText, size, _osdimage.height, _osdimage.width, wcharlen);
}
/*
 * 将时间转化为OSD图片
 * */
int Toolset::ConvertTimeToImage(OSDIMAGE& _osdimage, int size){
	char charText[100] = {0};
	GetNowTime(charText);

	_osdimage.width = size / 2 * strlen(charText);
	_osdimage.height= size;

	if(!AllocOsdImage(_osdimage)){
		printf("AllocOsdImage failed.\n");
		return -1;
	}

	if(!InitOsdImage(_osdimage, 0)) {
		printf("InitOsdImage failed.\n");
		return -1;
	}
	//char字符为wchar_t一半，20*（36/2）=360
	return Get_FT_BitMap(_osdimage.pimage, charText, size, _osdimage.height, _osdimage.width);
}

int Toolset::ConvertTimeToImage(char* charText, OSDIMAGE& _osdimage, int size){
	_osdimage.width = size / 2 * strlen(charText);
	_osdimage.height= size;

	if(!AllocOsdImage(_osdimage)){
		printf("AllocOsdImage failed.\n");
		return -1;
	}

	if(!InitOsdImage(_osdimage, 0)) {
		printf("InitOsdImage failed.\n");
		return -1;
	}
	//char字符为wchar_t一半，20*（36/2）=360
	return Get_FT_BitMap(_osdimage.pimage, charText, size, _osdimage.height, _osdimage.width);
}

int Toolset::Get_FT_BitMap(char**image,char* charText,int u32TextSize,int u32Height,int u32Width)
{
	FT_Library    library;
	FT_Face       face;

	FT_GlyphSlot  slot;
	FT_Matrix     matrix;
	FT_Vector     pen;
	FT_Error      error;

	error = FT_Init_FreeType(&library);  				//初始化freetye库
	if(error) {
		printf("error in FT_Init_FreeTypr\n");
		return error;
	}

  	error = FT_New_Face( library, configfile.c_str(), 0, &face ); //加载face对象
	if(error) {
		printf("error in FT_New_Face\n");
		return error;
	}
	FT_Set_Pixel_Sizes(face,u32TextSize, 0);		//设置字体大小
  	slot = face->glyph;
	int angle = (0.0/360)*3.14159*2;					//旋转度0.0
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	//字体起始坐标（0,2/3(heght)）
	pen.x = 5 * 64;
	pen.y = ( u32Height - u32Height*3/4 ) * 64;
	//循环变量
	int n;
	int i,j,p,q;
	//
	int x,y,x_max,y_max;
	for ( n = 0; n < strlen(charText); n++ )
	{
		FT_Set_Transform( face, &matrix, &pen );
		error = FT_Load_Char( face, charText[n], FT_LOAD_RENDER );		//转换位图
		if ( error ){
			printf("error in FT_Load_Char\n");
		  continue;
		}
		x = slot->bitmap_left;
		y = u32Height - slot->bitmap_top;
		x_max = x + slot->bitmap.width;
		y_max = y + slot->bitmap.rows;

		//写入 image
		for ( i = x,p = 0; i < x_max; i++, p++ )
		{
		    for ( j = y,q = 0; j < y_max; j++, q++ )
		    {
		      if ( i < 0 || j < 0 ||i >= u32Width || j >= u32Height )
		    	  continue;
		      image[j][i] |= slot->bitmap.buffer[q * slot->bitmap.width + p];
		    }
		}
		//控制字体格式
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
  	}
	//释放空间
	  FT_Done_Face    (face);
	  FT_Done_FreeType(library);
	  return 0;
}

//wchar_t型 轉換位圖
int Toolset::Get_FT_Wchar_BitMap(char **image,wchar_t* wcharText,int u32TextSize,int u32Height,int u32Width)//wchar_t型 轉換位圖
{
	FT_Library    library;
	FT_Face       face;
	FT_GlyphSlot  slot;
	FT_Matrix     matrix;
	FT_Vector     pen;
	FT_Error      error;

	error = FT_Init_FreeType(&library);  				//初始化freetye库
	if(error) {
		printf("error in FT_Init_FreeTypr\n");
		return error;
	}

  	error = FT_New_Face( library, configfile.c_str(), 0, &face ); //加载face对象
	if(error) {
		printf("error in FT_New_Face\n");
		return error;
	}
	FT_Set_Pixel_Sizes(face,u32TextSize, 0);		//设置字体大小
  	slot = face->glyph;
	int angle = (0.0/360)*3.14159*2;					//旋转度0.0
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	//字体起始坐标（5,3/4(heght)）
	pen.x = 5 * 64;
	pen.y = ( u32Height - u32Height*3/4 ) * 64;
	//循环变量
	int n;
	int i,j,p,q;
	//转换位图
	int x,y,x_max,y_max;
	for ( n = 0; n < wcslen(wcharText); n++ ) {
		FT_Set_Transform( face, &matrix, &pen );
		error = FT_Load_Char( face, wcharText[n], FT_LOAD_RENDER );
		if ( error ){
			printf("error in FT_Load_Char\n");
		  continue;
		}
	    //确定字体位置
		x = slot->bitmap_left;
		y = u32Height - slot->bitmap_top;
		x_max = x + slot->bitmap.width;
		y_max = y + slot->bitmap.rows;
		//写入 image
		for ( i = x,p = 0; i < x_max; i++, p++ ) {
		    for ( j = y,q = 0; j < y_max; j++, q++ ) {
		      if ( i < 0 || j < 0 ||i >= u32Width || j >= u32Height )//忽略超出部分
		    	  continue;
		      image[j][i] |= slot->bitmap.buffer[q * slot->bitmap.width + p];
		    }
		}
		//控制字体格式
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
  	}
	//释放空间
	FT_Done_Face    (face);
	FT_Done_FreeType(library);
	return 0;
}

int Toolset::Get_FT_Wchar_BitMap(char **image,wchar_t* wcharText,int u32TextSize,int u32Height,int u32Width, int u32Wsize)//wchar_t型 轉換位圖
{
	FT_Library    library;
	FT_Face       face;

	FT_GlyphSlot  slot;
	FT_Matrix     matrix;
	FT_Vector     pen;
	FT_Error      error;

	error = FT_Init_FreeType(&library);  				//初始化freetye库
	if(error) {
		printf("error in FT_Init_FreeTypr\n");
		return error;
	}

  	error = FT_New_Face( library, configfile.c_str(), 0, &face ); //加载face对象
	if(error) {
		printf("error in FT_New_Face\n");
		return error;
	}
	FT_Set_Pixel_Sizes(face,u32TextSize, 0);		//设置字体大小
  	slot = face->glyph;
	int angle = (0.0/360)*3.14159*2;					//旋转度0.0
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	//字体起始坐标（5,3/4(heght)）
	pen.x = 0 * 64;
	pen.y = ( u32Height - u32Height*0.8 ) * 64;
	//循环变量
	int n;
	int i,j,p,q;
	//转换位图
	int x,y,x_max,y_max;
	for ( n = 0; n < u32Wsize; n++ ) {
		FT_Set_Transform( face, &matrix, &pen );
		error = FT_Load_Char( face, wcharText[n], FT_LOAD_RENDER );
		if ( error ){
			printf("error in FT_Load_Char\n");
		  continue;
		}
	    //确定字体位置
		x = slot->bitmap_left;
		y = u32Height - slot->bitmap_top;
		x_max = x + slot->bitmap.width;
		y_max = y + slot->bitmap.rows;
		//写入 image
		for ( i = x,p = 0; i < x_max; i++, p++ ) {
		    for ( j = y,q = 0; j < y_max; j++, q++ ) {
		      if ( i < 0 || j < 0 ||i >= u32Width || j >= u32Height )//忽略超出部分
			  continue;
		      image[j][i] |= slot->bitmap.buffer[q * slot->bitmap.width + p];
		    }
		}
		//控制字体格式
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
  	}
	//释放空间
	FT_Done_Face (face);
	FT_Done_FreeType(library);
	return 0;
}
void Toolset::GetNowTime(char* timE)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timE,48,"%Y-%m-%d %H:%M:%S\n",timeinfo);
}

bool Toolset::utf8_to_unicode (char *inbuf, size_t *inlen, wchar_t **w_out, size_t *w_outlen)
{
    size_t outlen = (*inlen)*sizeof(wchar_t);
	char *outbuf = (char*)malloc(outlen);
	memset(outbuf, 0, outlen);
	char *encTo = "wchar_t";
	/* 源编码 */
	char *encFrom = "UTF-8";
	/* 获得转换句柄
	*@param encTo 目标编码方式
	*@param encFrom 源编码方式
	* */
	iconv_t cd = iconv_open (encTo, encFrom);
	if (cd == (iconv_t)-1)
	{
	  perror ("iconv_open");
	}
	/* 由于iconv()函数会修改指针，所以要保存源指针 */
	char *tmpin = inbuf;
	char *tmpout = outbuf;
	size_t insize = *inlen;
	size_t outsize = outlen;
	/* 进行转换
	*@param cd iconv_open()产生的句柄
	*@param srcstart 需要转换的字符串
	*@param inlen 存放还有多少字符没有转换
	*@param tempoutbuf 存放转换后的字符串
	*@param outlen 存放转换后,tempoutbuf剩余的空间
	*
	* */
	if (iconv (cd, &tmpin, inlen, &outbuf, &outlen) == -1){
	 perror ("iconv");
	}
	int w_size = outsize- outlen;
	*w_outlen = w_size/sizeof(wchar_t);
	*w_out = (wchar_t*)malloc(w_size);
	memset(*w_out, 0, w_size);
	memcpy(*w_out, tmpout, w_size);
	/* 关闭句柄 */
	iconv_close (cd);
	free(tmpout);

	return 0;
}

/*
 * AreaInfo.h
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#ifndef AREAINFO_H_
#define AREAINFO_H_
#include "BaseClass.h"

class AreaInfo : public BaseClass{
	public:
		class POINT{
		public:
			POINT() = delete;
			POINT(const int x, const int y);
			const int x = 0;
			const int y = 0;
		};
	public:
		~AreaInfo();
		AreaInfo();
		AreaInfo(int type);
		AreaInfo(const AreaInfo& area);
		int clear();
		POINT operator [](int index);

		void insertDone();
		bool isInsertDone();
		const int insertPoint(int x, int y);
		int getPoints(vector<POINT>& temp);

		int size();
		int getType();
		MyString toString();

		using BaseClass::operator=;
		AreaInfo& operator=(const AreaInfo& area);
#ifdef RAPIDJSON
		AreaInfo(MyString json);
		MyString toJson();
#endif
	private:
		int type = -1;
		int pointNum = 0;
		MyString name;
		DataContainer containerX;
		DataContainer containerY;
		bool insertDoneStatus = false;
};

#endif /* AREAINFO_H_ */

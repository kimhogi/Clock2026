#pragma once

#include "Data/sData/TemplateListSpread_2.hpp"
#include "ClockData.h"


using namespace std;
using namespace libxl;
using namespace ns_Weather;


class CWeatherSpread :public CTemplateListSpread<CWeatherDataList, CWeatherData>
{
public:
	CWeatherSpread() {};
	virtual ~CWeatherSpread() {};
	
private:
	CStatus* m_pStatus;

public:
	void SetStatus(CStatus* pStatus) { m_pStatus = pStatus; }

	void OnSetUp() override;

	void SetColumnsHeaderText(CFields& Fields) override;

	LRESULT EditModeOff(WPARAM wParam, LPARAM lParam);
	LRESULT LButtonClicked(CWeatherData& Object, WPARAM wParam, LPARAM lParam) override;

};
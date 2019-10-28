#include "Menu.h"
#include "../RenderSettings.h"
#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"
#include "../csv/csv.h"
#include "../celestial-sphere/CelestialSphere.h"
#include <iostream>

#include <sstream>

const int strToI(const std::string &str)
{
	std::stringstream s; s << str;
	int i; s >> i;
	return i;
}

const float MENU_WIDTH = 350.0f;

Menu::Menu(const sf::RenderWindow & window, RenderSettings & renderSettings, CelestialSphere &celestialSphere)
	: r_celestialSphere(celestialSphere), r_renderSettings(renderSettings), m_gui(window)
{
	initializeMenu(window);
	loadCities();

	// parse settings so Renderer can factor in time
	parseSettings();
}

void Menu::draw(sf::RenderWindow & window)
{
	m_gui.draw(window, r_renderSettings);
}

bool Menu::receiveInput(const sf::Event & evnt)
{
	return m_gui.receiveInput(evnt, r_renderSettings);
}

const sf::FloatRect Menu::getBounds() const
{
	return m_gui.getBounds();
}

const std::string timezoneToString(const int &minuteDifference)
{
	const bool positive = minuteDifference >= 0;
	const int minute = minuteDifference % 60;
	return (positive ? "+" : "") + std::to_string(minuteDifference / 60) + ":" + (minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute));
}

void Menu::initializeMenu(const sf::RenderWindow & window)
{
	sf::Vector2f menuLoc(window.getSize().x - MENU_WIDTH, 0.0f);

	m_gui.createItem("label1", MenuText(sf::Text("Press [F1] to toggle this menu", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 50.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label2", MenuText(sf::Text("Press [F2] to toggle star names", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 90.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label3", MenuText(sf::Text("Press [F3] to toggle gridlines", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 130.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label4", MenuText(sf::Text("Press [Esc] to unbind mouse", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 170.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label5", MenuText(sf::Text("Scroll or use up/down arrow keys", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 210.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label6", MenuText(sf::Text("  to simulate dimmer/brighter ", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 250.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label7", MenuText(sf::Text("  light conditions", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 290.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label8", MenuText(sf::Text("Press [Ctrl] and scroll or use", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 330.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label9", MenuText(sf::Text("  up/down arrow keys to zoom in", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 370.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label10", MenuText(sf::Text("  or out", m_gui.getFont(), 16U), sf::Vector2f{ 30.0f, 410.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("horizontalLine", HorizontalLine(sf::Vector2f{ 30.0f, 480.0f } *0.75f, MENU_WIDTH - 60.0f, menuLoc, sf::Color(59, 59, 59)));
	m_gui.createItem("label11", MenuText(sf::Text("Date", m_gui.getFont(), 16U), sf::Vector2f{ 35.0f, 530.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("label12", MenuText(sf::Text("Time", m_gui.getFont(), 16U), sf::Vector2f{ 35.0f, 580.0f } * 0.75f, menuLoc, sf::Color(184, 184, 184)));

	time_t now = time(0);
	tm *ltm = localtime(&now);

	m_gui.createTextBox("dateBox", MenuTextBox("dateBox", sf::Text(std::to_string(1 + ltm->tm_mon) + "/" + std::to_string(ltm->tm_mday) + "/" + std::to_string(1900 + ltm->tm_year)
		, m_gui.getFont(), 15U), sf::Vector2f{ 175.0f,520.0f } * 0.75f, sf::Vector2f(10.0f, 10.0f), menuLoc, sf::Color(124, 124, 124), sf::Color(19, 19, 19)))
		->connect([&]() {parseSettings(); });
	m_gui.createTextBox("timeBox", MenuTextBox("timeBox", sf::Text(std::to_string(ltm->tm_hour) + ":" + (ltm->tm_min < 10 ? "0" + std::to_string(ltm->tm_min) : std::to_string(ltm->tm_min))
		, m_gui.getFont(), 15U), sf::Vector2f{ 175.0f, 570.0f } *0.75f, sf::Vector2f(10.0f, 10.0f), menuLoc, sf::Color(124, 124, 124), sf::Color(19, 19, 19)))
		->connect([&]() {parseSettings(); });
	m_gui.createButton("resetTime", Button(sf::Text("Reset Time", m_gui.getFont(), 15U), sf::Vector2f{ 100.0f, 625.0f } *0.75f, { 150.0f,33.0f }, sf::Color(124, 124, 124), "resources/time.png", menuLoc))
		->connect([&] { resetTime(); });

	m_gui.createItem("latitude", MenuText(sf::Text("Latitude", m_gui.getFont(), 16U), sf::Vector2f{ 35.0f, 698.0f } *0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createItem("timezone", MenuText(sf::Text("Timezone", m_gui.getFont(), 16U), sf::Vector2f{ 35.0f, 748.0f } *0.75f, menuLoc, sf::Color(184, 184, 184)));
	m_gui.createTextBox("latBox", MenuTextBox("latBox", sf::Text("0", m_gui.getFont(), 15U), sf::Vector2f{ 175.0f, 688.0f } *0.75f, sf::Vector2f(10.0f, 10.0f), menuLoc, sf::Color(124, 124, 124), sf::Color(19, 19, 19)))
		->connect([&]() { parseSettings(); });
	m_gui.createTextBox("timezoneBox", MenuTextBox("timezoneBox", sf::Text("+0:00", m_gui.getFont(), 15U), sf::Vector2f{ 175.0f,738.0f } *0.75f, sf::Vector2f(10.0f, 10.0f), menuLoc, sf::Color(124, 124, 124), sf::Color(19, 19, 19)))
		->connect([&]() { parseSettings(); });
	m_gui.createItem("city", MenuText(sf::Text("City", m_gui.getFont(), 15U), sf::Vector2f{ 35.0f,698.0f } *0.75f, menuLoc, sf::Color(184, 184, 184)))->show = false;

	auto &cityBox = m_gui.createTextBox("cityBox", MenuTextBox("cityBox", sf::Text("none", m_gui.getFont(), 15U),
	sf::Vector2f{ 175.0f,688.0f } *0.75f, sf::Vector2f(10.0f, 10.0f), menuLoc, sf::Color(124, 124, 124), sf::Color(19, 19, 19), 999));

	cityBox->show = false;
	cityBox->connect([&] {parseSettings(); });

	m_gui.createItem("cityInfo", MenuText(sf::Text("", m_gui.getFont(), 14U), sf::Vector2f{ 35.0f,738.0f } *0.75f, menuLoc, sf::Color(59, 59, 59)))->show = false;
	m_gui.createButton("locateCity", Button(sf::Text("Locate City", m_gui.getFont(), 15U), sf::Vector2f{ 100.0f,793.0f } *0.75f, { 150.0f,33.0f }, sf::Color(124, 124, 124), "resources/locate.png", menuLoc))
		->connect([&] {toggleLocator(); });
	m_gui.createItem("errorMessage", MenuText(sf::Text("", m_gui.getFont(), 13U), sf::Vector2f{ 30.0f,843.0f } *0.75f, menuLoc, sf::Color(237, 35, 52)));
}

void Menu::loadCities()
{
	io::CSVReader<4> in("resources/world-cities.csv");
	struct CSVData { std::string name; double lat, lon; std::string timezone; };
	std::vector<CSVData> csvData;

	//for (int i = 0; i < 120000; i++) csvData.push_back({ "asdf", 0.,(double)i,"10:00" });

	in.read_header(io::ignore_no_column, "Name", "Latitude", "Longtitude", "Timezone");

	std::string cityName, timezone;
	double lat, lon;

	int counter = 0;

	try
	{
		while (in.read_row(cityName, lat, lon, timezone))
		{
			//if (counter > 1000) break;
			std::string cityStr = su::toLower(cityName);
			const auto slash = cityStr.find('/');

			if (slash != std::string::npos) cityStr = cityStr.substr(0, slash);

			//csvData.push_back({ cityStr,lat,lon,timezone });
			//if (timezone[0] != '+') std::cout << timezone << std::endl;
			m_cityInfo[cityStr] = { (float)lat,timezone };

			counter++;
		}
	}
	catch (const io::error::too_many_columns &e)
	{
		std::cout << std::endl << e.file_line;
	}

}

void Menu::parseSettings()
{
	m_gui.getText("errorMessage")->setString(calculateGST());
}

const std::string Menu::calculateGST()
{
	std::vector<std::string> dateStrings = su::split(m_gui.getTextBox("dateBox")->getValue(), '/');

	if (dateStrings.size() != 3) return "Invalid date format";

	for (const std::string &s : dateStrings) if (!su::isDigit(s)) return "Invalid date value '" + s + "'";

	float mo = su::strToF(dateStrings[0]),
		d = su::strToF(dateStrings[1]),
		yr = su::strToF(dateStrings[2]);

	//std::cout << "\nDate : " << mo << "/" << d << "/" << yr;

	std::vector<std::string> hourStrings = su::split(m_gui.getTextBox("timeBox")->getValue(), ':');

	if (hourStrings.size() != 2) return "Invalid time format";

	for (const std::string &s : hourStrings) if (!su::isDigit(s)) return "Invalid time value '" + s + "'";

	int hr = su::strToD(hourStrings[0]), min = su::strToD(hourStrings[1]);

	//std::cout << "\nTime : " << hr << ":" << min;

	float lat;
	int minuteDifference;

	if (usingCities)
	{
		const std::string city = m_gui.getTextBox("cityBox")->getValue();

		auto &find = m_cityInfo.find(su::toLower(city));

		if (find != m_cityInfo.end())
		{
			CityInfo city = find->second;
			auto timezoneStrings = su::split(city.timezoneStr, ':');

			int hour = strToI(timezoneStrings[0]), minute = strToI(timezoneStrings[1]);
			int minuteDiff;
			if (hour < 0) minuteDiff = hour * 60 - minute;
			else minuteDiff = hour * 60 + minute;

			std::cout << "\nFound city! " << city.latitude << ", " << minuteDiff;
			m_gui.getText("cityInfo")->setString("(" + std::to_string(find->second.latitude) + ", " + city.timezoneStr + ")");
			lat = find->second.latitude;
			minuteDifference = minuteDiff;		// TODOODODODODOODO
		}
		else
		{
			return "Could not find '" + city + "'";
		}
	}
	else
	{
		std::string strLat = m_gui.getTextBox("latBox")->getValue(), strZone = m_gui.getTextBox("timezoneBox")->getValue();

		if (!su::isFloat(strLat)) return "Invalid latitude value '" + strLat + "'";
		if (strZone.length() == 0 || (strZone[0] != '+' && strZone[0] != '-')) return "Invalid timezone format";

		bool positive = strZone[0] == '+';
		strZone = strZone.substr(1);

		auto timezoneStrings = su::split(strZone, ':');

		if (timezoneStrings.size() != 2) return "Invalid timezone format";
		for (const std::string &value : timezoneStrings) if (!su::isDigit(value)) return "Invalid timezone value " + value;

		int hour = su::strToD(timezoneStrings[0]), minute = su::strToD(timezoneStrings[1]);
		minuteDifference = hour * 60 + minute;
		if (!positive) minuteDifference = -minuteDifference;

		lat = su::strToF(m_gui.getTextBox("latBox")->getValue());

		// reset values in case there were characters in strings //
		m_gui.getTextBox("latBox")->setValue(std::to_string(lat + 0.0f));
		m_gui.getTextBox("timezoneBox")->setValue(timezoneToString(minuteDifference));
	}


	// reset values in case there were characters in strings //
	m_gui.getTextBox("dateBox")->setValue(std::to_string((int)mo) + "/" + std::to_string((int)d) + "/" + std::to_string((int)yr));
	m_gui.getTextBox("timeBox")->setValue(std::to_string(hr) + ":" + (min < 10 ? "0" + std::to_string(min) : std::to_string(min)));
	
	tm set = { 0,min,hr,d,mo - 1,yr - 1900 };
	/*

	tm gmt = mu::toGreenwichMeanTime(set, lon);

	float lst = mu::getGreenwichSidrealTime(gmt) + (lon / 15.0f);
	while (lst < 0.0f) lst += 24.0f;
	while (lst > 24.0f) lst -= 24.0f;

	std::cout << "Greenwich time : " << asctime(&gmt) << std::endl;
	std::cout << "Greenwich Sidreal Time : " << su::toString(mu::getGreenwichSidrealTime(gmt)) << std::endl;
	std::cout << "Local Sidreal Time : " << su::toString(lst) << std::endl;*/

	r_celestialSphere.update(set, lat, minuteDifference);
	/*
	r_renderSettings.skyOffset.x = -(lst / 24.0f * 2.0f * mu::PI);
	r_renderSettings.skyOffset.y = -((90.0f - lat) / 360.0f * 2.0f * mu::PI);*/

	return "";		// no errors in calculating gst, set error message to none
}

void Menu::resetTime()
{
	std::cout << "\nResetting time...";

	time_t now = time(0);
	tm *ltm = localtime(&now);

	m_gui.getTextBox("dateBox")->setValue(std::to_string(1 + ltm->tm_mon) + "/" + std::to_string(ltm->tm_mday) + "/" + std::to_string(1900 + ltm->tm_year));
	m_gui.getTextBox("timeBox")->setValue(std::to_string(ltm->tm_hour) + ":" + (ltm->tm_min < 10 ? "0" + std::to_string(ltm->tm_min) : std::to_string(ltm->tm_min)));

	calculateGST();
}

void Menu::toggleLocator()
{
	usingCities = !usingCities;

	m_gui.getItem<MenuItem>("latBox")->toggleShow();
	m_gui.getItem<MenuItem>("timezoneBox")->toggleShow();
	m_gui.getItem<MenuItem>("latitude")->toggleShow();
	m_gui.getItem<MenuItem>("timezone")->toggleShow();
	m_gui.getItem<MenuItem>("cityBox")->toggleShow();
	m_gui.getItem<MenuItem>("city")->toggleShow();
	m_gui.getItem<MenuItem>("cityInfo")->toggleShow();

	if (usingCities) m_gui.getItem<Button>("locateCity")->setLabel("Coordinates");
	else m_gui.getItem<Button>("locateCity")->setLabel("Locate City");
}
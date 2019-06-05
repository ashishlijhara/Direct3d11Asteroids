#include "Score.h"
#include <cstring>
#include <algorithm>
#include <fstream>
#include <string>

#define HS_CSV "highscore.csv"

Score* Score::instance_ = NULL;

Score::Score() {
	ResetScore();
	LoadScore();
}

Score *Score::GetScoreKeeper()
{
	if (!instance_)
		instance_ = new Score();
	return instance_;
}

void Score::DestroyScoreKeeper() {
	delete instance_;
	instance_ = NULL;
}

void Score::ResetScore() {
	memset(&score_, 0, sizeof(ScoreData));
}

const ScoreData& Score::GetScore() const {
	return score_;
}

void Score::UpdateScore(const int & level, const int& score) 
{
	score_.level = level;
	score_.score = score;
}

void Score::SetHighScore() const{
	highscore_.push_back(score_);
	std::sort(highscore_.begin(), highscore_.end(), [](const ScoreData& a, const ScoreData& b) {return ((a.score) < (b.score)); });
	std::reverse(std::begin(highscore_), std::end(highscore_));
	highscore_.resize(3);
	std::ofstream csv;
	csv.open(HS_CSV);
	for (ScoreDataList::const_iterator list = highscore_.begin(), end = highscore_.end(); list != end; list++)
		WriteToCSV(csv, *list);
}

const ScoreDataList& Score::GetHighScore() const{
	return highscore_;
}

void Score::Tokenize(std::string const &str, const char delim, std::vector<std::string> &out) const
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

void Score::LoadScore() const{
	std::ifstream icsv;
	icsv.open(HS_CSV);  
	if (!icsv.is_open())
	{
		std::ofstream csv;
		csv.open(HS_CSV, std::ofstream::out | std::ofstream::app);
		for (int i = 0; i < 3; i++)
		{
			ScoreData data;
			highscore_.push_back(data);
			WriteToCSV(csv, data);
		}
	}
	else
	{
		while (icsv.eof() == 0)
		{
			char str[10];
			icsv.getline(str, 10);

			std::string s = std::string(str);
			size_t pos = 0;

			std::vector<std::string> out;
			Tokenize(s, ',', out);
			if (out.size() > 0)
			{
				ScoreData data(std::stoi(out[0]), std::stoi(out[1]));
				highscore_.push_back(data);
			}
		}
	}
	icsv.close();
}



void Score::WriteToCSV(std::ofstream& csv, const ScoreData& data) const
{
	csv << data.level << "," << data.score << "," << std::endl;
}


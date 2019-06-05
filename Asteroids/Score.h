//#pragma once
#ifndef SCORE_H
#define SCORE_H


#include <vector>

using namespace std;

struct ScoreData {
	int level;
	int score;

	ScoreData() :score(0), level(0) {}

	ScoreData(const int& _level, const int& _score) :score(_score), level(_level) {}
};

typedef vector<ScoreData> ScoreDataList;

//Singleton Score class
class Score
{
public:

	static Score *GetScoreKeeper();
	static void DestroyScoreKeeper();

	void ResetScore();

	const ScoreData& GetScore() const;

	//Persistant Highscore
	void SetHighScore() const;

	const ScoreDataList& GetHighScore() const;

	void UpdateScore(const int&, const int&);

private:

	Score();
	Score(const Score&);
	Score& operator= (const Score &) {}
	~Score() {}

	void LoadScore() const;

	void WriteToCSV(std::ofstream&, const ScoreData&) const;
	void Tokenize(std::string const &, const char, std::vector<std::string> &) const;

	ScoreData score_;
	static Score *instance_;

	mutable ScoreDataList highscore_;
};

#endif // !SCORE_H
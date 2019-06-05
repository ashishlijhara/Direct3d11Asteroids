#include "HighScore.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "Game.h"
#include "Score.h"


HighScore::HighScore():delay_(0)
{
}

void HighScore::OnActivate(System * system, StateArgumentMap & args)
{
	delay_ = 180;
}

void HighScore::OnUpdate(System * system)
{
	if (--delay_ == 0)
	{
		system->SetNextState("MainMenu");
	}
}

void HighScore::OnRender(System *system)
{
	Graphics *graphics = system->GetGraphics();
	FontEngine *fontEngine = graphics->GetFontEngine();

	system->GetGame()->RenderBackgroundOnly(graphics);

	const char *highScoreText = "High Score";
	int textWidth = fontEngine->CalculateTextWidth(highScoreText, FontEngine::FONT_TYPE_LARGE);
	int textHeight = fontEngine->CalculateTextHeight(highScoreText, FontEngine::FONT_TYPE_LARGE);
	int textX = (800 - textWidth) / 2;
	int textY = 22;
	fontEngine->DrawText(highScoreText, textX, textY, 0xff00ffff, FontEngine::FONT_TYPE_LARGE);
	
	const char *levelText = "Level     Score";
	textWidth = fontEngine->CalculateTextWidth(levelText, FontEngine::FONT_TYPE_MEDIUM);
	textX = (800 - textWidth) / 2;
	textY += textHeight + 20;
	fontEngine->DrawText(levelText, textX, textY, 0xffffffff, FontEngine::FONT_TYPE_MEDIUM);
	textHeight = fontEngine->CalculateTextHeight(levelText, FontEngine::FONT_TYPE_MEDIUM);

	ScoreDataList scoreData = Score::GetScoreKeeper()->GetHighScore();
	for (ScoreDataList::const_iterator it = scoreData.begin(), end = scoreData.end(); it != end; it++) {
		std::string s = std::to_string((*it).level);
		s.append("         ");
		s.append(std::to_string((*it).score));
		
		const char* scoreText = s.c_str();
		textWidth = fontEngine->CalculateTextWidth(scoreText, FontEngine::FONT_TYPE_MEDIUM);
		textX = (800 - textWidth) / 2;
		textY += textHeight + 20;
		fontEngine->DrawText(scoreText, textX, textY, 0xffff00ff, FontEngine::FONT_TYPE_MEDIUM);
		textHeight = fontEngine->CalculateTextHeight(scoreText, FontEngine::FONT_TYPE_MEDIUM);
	}
}

void HighScore::OnDeactivate(System * system)
{

}

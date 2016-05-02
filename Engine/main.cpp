#include "Main/GameEngine.h"
#include "Main/Game.h"
#include "Arguments/ArgumentsAnalyzer.h"

int main(int argc, char **argv) 
{
	ArgumentsAnalyzer::Instance ()->ProcessArguments (argc, argv);

	GameEngine::Init ();
	
	Game::Instance ()->Start ();

	GameEngine::Clear ();
}

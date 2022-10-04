# R-Type

Hola bb

main > Game > Scene > Player

GAME_STATE = {MENU, CREDITS, GAME, INFO}

Game::Update() {
	switch(state) {
		case CREDITS: 	if (key_up) active_button--;
		case MENU: 	if (space && ...) state=GAME;
		case GAME: 	if (key_up) space.y++;
	}
}

Game::Render() {
	switch(state) {
		case CREDITS: 	DrawCredits();
		case GAME: 	DrawScene(); Draw_player();
	}
}

trying PR.
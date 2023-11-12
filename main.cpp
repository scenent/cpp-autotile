#include "main.h"
using namespace rc;

const std::string autotileImagePath = get_current_path() + "\\autotile.png";

Camera cam = Camera();
AutoTile* autotile = nullptr;


void process(double deltaTime, FrameEvent event) {
	if (event.justKeys[KEY_SPACE].action == RC_PRESS) {
		autotile->clear();
		for (int i = 0; i < 500; i++) {
			autotile->put(int(get_rand(0.0, 24.0)), int(get_rand(0.0, 13.0)));
		}
	}
}

void draw() {
	autotile->draw();
}

int main() {
	srand(time(0));
	if (init_window("hello world", vec2(160, 90), vec2(1600, 900)) != WINDOW_CONDITION::SUCCESS) {
		return EXIT_FAILURE;
	}
	autotile = new AutoTile(autotileImagePath, vec2(64, 64));

	for (int i = 0; i < 500; i++) {
		autotile->put(int(get_rand(0.0, 24.0)), int(get_rand(0.0, 13.0)));
	}

	register_process_func(process, draw);
	set_background_color({ 0.31, 0.31, 0.31, 1.0 });
	start_main_loop();
	delete autotile;
	uninit_window();
}
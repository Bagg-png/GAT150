#include "Game.h"

int main(int, char**) {
	Game game;
	ag::Timer timer;
	ag::Engine engine;
	ag::Scene scene;
	scene.engine = &engine;

	game.Initialize();

	/*engine.StartUp();
	engine.Get<ag::Renderer>()->Create("GAME", 800, 600);*/

	//std::cout << timer.ElapsedTicks() << std::endl;


	//ag::SetFilePath("../Resources");

	/*engine.Get<ag::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine.Get<ag::AudioSystem>()->AddAudio("music", "audio/song.mp3");
	ag::AudioChannel channel = engine.Get<ag::AudioSystem>()->PlayAudio("music", 1,1,true);

	int size = 16;
	std::shared_ptr<ag::Font> font = engine.Get<ag::ResourceSystem>()->Get<ag::Font>("Fonts/3dumb.ttf", &size);
	std::shared_ptr<ag::Texture> textTexture = std::make_shared<ag::Texture>(engine.Get<ag::Renderer>());
	textTexture->Create(font->CreateSurface("hello world", ag::Color{ 1,1,1 }));
	engine.Get<ag::ResourceSystem>()->Add("textTexture", textTexture);

	std::shared_ptr<ag::Texture> texture = engine.Get<ag::ResourceSystem>()->Get<ag::Texture>("sf2.png", engine.Get<ag::Renderer>());
	for (size_t i = 0; i < 10; i++) {
		ag::Transform transform{ {ag::RandomRange(0,800), ag::RandomRange(0,600)}, ag::RandomRange(0,360), 1.0f };
		std::unique_ptr<ag::Actor> actor = std::make_unique<ag::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}*/

	//SDL_Point screen{ 800,600 };

	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3.0f;

	while (!quit && !game.IsQuit()) {
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		}
		game.Update();
		game.Draw();
		

		//std::cout << engine.time.time << std::endl;
		//if (engine.time.time >= quitTime) quit = true;
		//engine.time.timeScale = 2;

		
		/*for (size_t i = 0; i < 50; i++) {
			SDL_Rect src{32,64,32,64};
		SDL_Rect dest{ag::RandomRangeInt(0,screen.x),ag::RandomRangeInt(0,screen.x),32,48};
		SDL_RenderCopy(renderer, texture, &src, &dest);
		}*/
		
	}
	SDL_Quit();
	return 0;
}
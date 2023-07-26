#include "Graphyy.h"

std::vector<double> extractValsFromPtrVector(std::vector<double*> ptrVector)
{
	std::vector<double> v{};
	for (int i = 0; i < ptrVector.size(); i++)
		v.push_back(*ptrVector[i]);
	return v;
}

double evaluateFunction(std::vector<double*> vals, bool* doesError)
{
	return GraphyyFunc(extractValsFromPtrVector(vals), doesError);
}

double limit(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

int evalAsColorValue(double eval, bool canGoNegative)
{
	double colorValue = eval * (canGoNegative ? 127 : 255);
	return (int)colorValue + (canGoNegative ? 128 : 0);
}

bool drawFunctionMap(SDL_Renderer* renderer, const int wWidth, const int wHeight, std::vector<double*> vals, double* xPtr, double* yPtr)
{
	for (int y = 0; y < wHeight; y++)
	{
		for (int x = 0; x < wWidth; x++)
		{
			*xPtr = x; *yPtr = y;
			bool doesError = false;

			double eval = limit(evaluateFunction(vals, &doesError), -1, 1);
			if (doesError) return false;

			int colorValue = evalAsColorValue(eval, true);
			SDL_SetRenderDrawColor(renderer, colorValue, colorValue, colorValue, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}

	return true;
}

const char* graphyyErr = "";

int main(int argc, char** argv)
{
	std::cout << "Graphyy v" << GRAPHYY_VER;
#ifdef GRAPHYY_DEV
	std::cout << " [DEV BUILD " << std::to_string(GRAPHYY_DEV) << "]";
#endif // GRAPHYY_DEV
	std::cout << "\n";

	std::vector<double*> vals{};
	double normalVals[128]{};
	double x = 0;
	double y = 0;
	for (int i = 1; i < argc; i++) {
		char* arg = argv[i];

#ifdef GRAPHYY_DEV
		std::cout << "Arg " << std::to_string(i) << ": '" << arg << "'\n";
#endif // GRAPHYY_DEV

		double d = 0;
		int val = sscanf_s(arg, "%lf", &d);
		if (!val) {
#ifdef GRAPHYY_DEV
			std::cout << "-> this isn't a valid decimal num\n";
#endif // GRAPHYY_DEV
			switch (std::tolower(arg[0])) {
			case 'x':
#ifdef GRAPHYY_DEV
				std::cout << "-> it's the x position, i will remember that\n";
#endif // GRAPHYY_DEV
				vals.push_back(&x);
				break;
			case 'y':
#ifdef GRAPHYY_DEV
				std::cout << "-> it's the y position, i will remember that\n";
#endif // GRAPHYY_DEV
				vals.push_back(&y);
				break;
			default:
#ifdef GRAPHYY_DEV
				std::cout << "-> it's some random string, welp time to shit myself\n"; // it's a monday night what the fuck am i doing
#endif // GRAPHYY_DEV
				std::cout << "'" << arg << "' is not a decimal number nor a valid constant\n";
				return 1;
			}
		} else {
			// this is just so hacky gah dayum
			normalVals[i] = *&d;
			vals.push_back(&normalVals[i]);
		}
	}

#ifdef GRAPHYY_DEV
	for (int i = 0; i < vals.size(); i++) {
		double val = *vals[i];
		std::cout << "Val index " << std::to_string(i) << ": " << std::to_string(val);
		std::cout << " (x?" << (vals[i] == &x ? "yay" : "nay");
		std::cout << " y?" << (vals[i] == &y ? "yay" : "nay") << ")";
		std::cout << "\n";
	}
#endif // GRAPHYY_DEV

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Window* win = SDL_CreateWindow(
		"Graphyy",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_SHOWN);
	if (!win) { 
		std::cout << "Failed to create window: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cout << "Failed to create renderer: " << SDL_GetError() << "\n";
		return 1;
	}

	TTF_Font* roboto = TTF_OpenFont("assets/Roboto/Roboto-Regular.ttf", 24);

	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, NULL);

		if (!drawFunctionMap(renderer, WINDOW_W, WINDOW_H, vals, &x, &y))
		{
			running = false;
			std::cout << "An error occurred while drawing the function's map.\n";
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	TTF_CloseFont(roboto);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	return 0;
}
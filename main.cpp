#include "UImanager.h"
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

std::string FONTTYPESPATH = "C:\\Windows\\Fonts\\";
std::string exten = ".ttf";
std::string fontname = "Bahnschrift";

const char*  UIfontPath = "C:\\Windows\\Fonts\\Bahnschrift.ttf";

std::string FILENAME;


int currentTbox = 2;



SDL_Color deselected = {150,150,150,255};
SDL_Color Active = {106,106,106,255};
SDL_Color Pressed = { 0,0,0,255 };
SDL_Color White = { 255,255,255,255 };

UImanager::FontStyle UIstyle = { 24,SDL_Color{0,0,0,0},UIfontPath };
UImanager::FontStyle PanelStyle = { 24,White,UIfontPath };
UImanager::FontStyle textstyle = { 14,SDL_Color{0,0,0,0},UIfontPath };

UImanager::Button SaveButton;

UImanager::Button Openbutton;

UImanager::Button textfeild;

UImanager::Button fontfeild;

UImanager::Button FontButton;

UImanager::Button savefeild;

UImanager::Button openfeild;


UImanager::panel DebuggerPanel;




std::vector<UImanager::Button> buttons;

int Sbegin,SEnd;


TTF_Font* font;




void SaveText()
{
	std::ofstream myfile;
	myfile.open(buttons[5].message.c_str());

	myfile << buttons[2].message;
	myfile.close();
}

void OpenText()
{
	std::string line;
	std::ifstream myfile(buttons[6].message.c_str());
	if (myfile.is_open())
	{
		buttons[2].message = "";
		while (getline(myfile, line))
		{
			buttons[2].message += line + '\n';
		}
		myfile.close();
	}

	else DebuggerPanel.text = "Failed to read file path is wrong or file has an incorrect format";
}

void OpenFont()
{

	std::ifstream myfile(FONTTYPESPATH + buttons[3].message + exten);
	if (!myfile.good())
	{
		textstyle.style = UIfontPath;
		DebuggerPanel.text = "Failed to read font will reset to the default font";
		return;
	}
	textstyle.style = (FONTTYPESPATH + buttons[3].message + exten).c_str();
	
	
}

void StartText()
{


	SDL_StartTextInput();
	



}

void EndText()
{
	SDL_StopTextInput();
	
}

std::map<std::string, std::function<void()>>  funcMap =
{	{ "Save", SaveText},
	{ "Open", OpenText},
	{"StartText",StartText},
	{"EndText",EndText},
	{"OpenFont",OpenFont}
};

void DrawUIText(const char* msg,int x,int y,int wrap,UImanager::FontStyle* fntstyle,SDL_Renderer* ren) {
	font = TTF_OpenFont(fntstyle->style, fntstyle->size);
	SDL_Surface* surf;
	SDL_Texture* tex;
	SDL_Color color = fntstyle->color;
	SDL_Rect rect;
	if (!font)
	{
		DebuggerPanel.text = "Failed to read font will reset to the default font";
	}
	surf = TTF_RenderText_Blended_Wrapped(font, msg, color,wrap);
	tex = SDL_CreateTextureFromSurface(ren, surf);
	if (!surf || !tex)
	{
		return;
	}
	rect.x = x;
	rect.y = y;
	rect.w = surf->w;
	rect.h = surf->h;
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	SDL_RenderCopy(ren, tex, NULL, &rect);
	
	SDL_DestroyTexture(tex);
}

void RenderButton(SDL_Surface* screen, SDL_Window* win, UImanager::Button* b, SDL_Renderer* rnd, UImanager::Vector2i p, bool mclick,int index)
{

	SDL_Texture* tex;


	SDL_Color col;

	col.r = b->RegularCol->r;
	col.g = b->RegularCol->g;
	col.b = b->RegularCol->b;
	col.a = b->RegularCol->a;

	switch (mclick)
	{
	case false:
		switch (UImanager::PointInRect(p, &b->r))
		{
		case false:
			col.r = b->RegularCol->r;
			col.g = b->RegularCol->g;
			col.b = b->RegularCol->b;
			col.a = b->RegularCol->a;
			break;
		default:
			col.r = b->ActiveCol->r;
			col.g = b->ActiveCol->g;
			col.b = b->ActiveCol->b;
			col.a = b->ActiveCol->a;
			break;
		}
		break;
	default:
		switch (UImanager::PointInRect(p, &b->r))
		{
		case true:
			col.r = b->PressedCol->r;
			col.g = b->PressedCol->g;
			col.b = b->PressedCol->b;
			col.a = b->PressedCol->a;
			if (b->action == "StartText")
			{
				currentTbox = index;
			}
			funcMap[b->action]();
			break;
		default:
			if (!b->failAction.empty())
			{
			 funcMap[b->failAction]();
			}
				
			break;
		}
	}

	
	SDL_SetRenderDrawColor(rnd, col.r, col.g, col.b, 255);
	SDL_RenderFillRect(rnd, &b->r);


	if (b->message == "")
	{
		return;
	}

	if (!b->Fstyle)
	{
	
		DrawUIText(b->message.c_str(), b->r.x, b->r.y, b->r.w,&UIstyle,rnd);
		return;
	}

	DrawUIText(b->message.c_str(), b->r.x, b->r.y, b->r.w,b->Fstyle, rnd);
	

}

void RenderPanel(UImanager::panel* p,SDL_Surface* screen, SDL_Window* win, SDL_Renderer* rnd)
{
	SDL_SetRenderDrawColor(rnd,p->col->r, p->col->g, p->col->b, 255);
	SDL_RenderFillRect(rnd, &p->r);
	DrawUIText(p->text.c_str(), p->r.x, p->r.y, p->r.w,p->Fstyle, rnd);
}





void Refresh(SDL_Surface* screen , SDL_Window* win,SDL_Renderer* renderer , UImanager::Vector2i mpos, bool mclick)
{ 
	

	SDL_Rect ribbon = { 0,0,SCREEN_WIDTH,110 };
	SDL_SetRenderDrawColor(renderer, 200,200, 200, 255);
	SDL_RenderFillRect(renderer,&ribbon);


	for (int i = 0; i < buttons.size(); i++)
	{
		RenderButton(screen, win, &buttons[i], renderer, mpos, mclick,i);
	}
	

	RenderPanel(&DebuggerPanel, screen, win, renderer);

	
	
	
	
}



void HandleInput(SDL_Event* ev,UImanager::Button* b)
{
	switch (ev->type)
	{
	case SDL_TEXTINPUT:
		b->message += ev->text.text;
		break;
	case SDL_KEYDOWN:
		if (b->message.length() < 0)
		{
			break;
		}
		switch (ev->key.keysym.sym)

		{
		case SDLK_BACKSPACE:
			b->message = b->message.substr(0, b->message.length() - 1);
			break;
		case SDLK_RETURN:
			b->message += '\n';
			break;
		case SDLK_EXCLAIM:
			b->message += '!';
			break;
		case SDLK_LCTRL:
			
			buttons[2].message = "";
			break;
		}
	}

}


int main(int argc, char** argv) {


	SaveButton.r.x = 32;
	SaveButton.r.y = 24;
	SaveButton.r.w = 128;
	SaveButton.r.h = 34;
	SaveButton.RegularCol = &deselected;
	SaveButton.ActiveCol = &Active;
	SaveButton.PressedCol = &Pressed;
	SaveButton.message = "Save";
	SaveButton.action = "Save";

	Openbutton.r.x = 170;
	Openbutton.r.y = 24;
	Openbutton.r.w = 128;
	Openbutton.r.h = 34;
	Openbutton.RegularCol = &deselected;
	Openbutton.ActiveCol = &Active;
	Openbutton.PressedCol = &Pressed;
	Openbutton.message = "Open";
	Openbutton.action = "Open";

	textfeild.r.x = 128;
	textfeild.r.y = 128;
	textfeild.r.w = 768;
	textfeild.r.h = 1024;
	textfeild.Fstyle = &textstyle;
	textfeild.RegularCol = &White;
	textfeild.ActiveCol = &White;
	textfeild.PressedCol = &White;
	textfeild.message = "";
	textfeild.action = "StartText";
	textfeild.failAction = "EndText";

	fontfeild.r.x = 340;
	fontfeild.r.y = 64;
	fontfeild.r.w = 156;
	fontfeild.r.h = 34;
	fontfeild.RegularCol = &White;
	fontfeild.ActiveCol = &White;
	fontfeild.PressedCol = &White;
	fontfeild.message = fontname;
	fontfeild.action = "StartText";
	fontfeild.failAction;

	FontButton.r.x = 340;
	FontButton.r.y = 24;
	FontButton.r.w = 128;
	FontButton.r.h = 34;
	FontButton.RegularCol = &deselected;
	FontButton.ActiveCol = &Active;
	FontButton.PressedCol = &Pressed;
	FontButton.message = "Load";
	FontButton.action = "OpenFont";

	savefeild.r.x = 32;
	savefeild.r.y = 64;
	savefeild.r.w = 128;
	savefeild.r.h = 34;
	savefeild.RegularCol = &White;
	savefeild.ActiveCol = &White;
	savefeild.PressedCol = &White;
	savefeild.message = "";
	savefeild.action = "StartText";
	savefeild.failAction;

	openfeild.r.x = 170;
	openfeild.r.y = 64;
	openfeild.r.w = 128;
	openfeild.r.h = 34;
	openfeild.RegularCol = &White;
	openfeild.ActiveCol = &White;
	openfeild.PressedCol = &White;
	openfeild.message = "";
	openfeild.action = "StartText";
	openfeild.failAction;

	DebuggerPanel.col = &Active;
	DebuggerPanel.Fstyle = &PanelStyle;
	DebuggerPanel.r = {0 ,976,1024,48 };
	DebuggerPanel.text = "Welcome !";
	buttons.push_back(SaveButton);
	buttons.push_back(Openbutton);
	buttons.push_back(textfeild);
	buttons.push_back(fontfeild);
	buttons.push_back(FontButton);
	buttons.push_back(savefeild);
	buttons.push_back(openfeild);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Typewriter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) {
		printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
		return 1;
	}
	SDL_Surface* screen;
	screen = SDL_GetWindowSurface(window);
	TTF_Init();
	bool running = true;
	
	SDL_Color col;
	col.r = 255;
	col.g = 0;
	col.b = 0;
	col.a = 0;
	SDL_Rect rec;
	rec.x = 20;
	rec.y = 30;
	rec.w = SCREEN_WIDTH;
	rec.h = SCREEN_HEIGHT;

	

	StartText();
	
	//SDL_Init(SDL_INIT_EVERYTHING);
	while (running) {
		SDL_Event event;



		



		


		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				TTF_Quit();
				running = false;
				break;

				
					
				
				break;
			default:
				break;
			}
			HandleInput(&event,&buttons[currentTbox]);
			int x, y;
			SDL_GetMouseState(&x, &y);

			SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
			SDL_RenderClear(renderer);
			Refresh(screen, window,renderer, UImanager::Vector2i{x,y},event.type == SDL_MOUSEBUTTONDOWN );
			
			SDL_RenderPresent(renderer);
		}

		//SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

	}
	
	SDL_Quit();
	return 0;
}


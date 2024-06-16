#include "main.cpp"
#define pressed(b) input.buttons[b].is_down && input.buttons[b].changed
#define released(b) !input.buttons[b].is_down && input.buttons[b].changed
#define is_down(b) input.buttons[b].is_down
const float scale = 0.5f;
sf::Texture grid;
sf::Sprite grid_sp;
sf::Vector2i mouse;
int selected = -10;
float radius=0;
Level levels[10];
Square starting_square;
std::vector<LinkThread> completed_links = {};
float distance=0;
bool isSelected = false;
int cur_level = 0;
GridData current_color;
std::vector<Link> links = {};
std::vector<Square> selcted_squares = {};
Square prev_square = {-10,0,EMPTY};
std::vector<Square> completed_squares = {};
sf::Color to_color(GridData gridData) {
	switch (gridData) {
	case RED:
		return sf::Color::Red;
	case BLUE:
		return sf::Color::Blue;
	case GREEN:
		return sf::Color::Green;
	case PINK: {
		return sf::Color::Magenta;
	}
	default:
		std::cout << "Invalid grid data in to_color()"<<gridData<<"\n";
		return sf::Color::White;
	}
}
Square getSquare(int index) {
	int x = index % levels[cur_level].grid;
	int y = (index - x) / levels[cur_level].grid;

	Square square;
	square.index = index;
	square.isFilled = (levels[cur_level].level_map[y][x] != EMPTY);
	square.color = levels[cur_level].level_map[y][x];
	return square;

}
void updateLevels() {
	//Level 1
	levels[0].grid = 7;
	levels[0].level_map = {
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{GREEN,RED,EMPTY,EMPTY,EMPTY,GREEN,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,RED,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
	};
	levels[0].circles = {};
}
Square getCurrentSquare() {
	sf::Vector2f grid_pos = grid_sp.getPosition();
	distance = (grid.getSize().x / levels[cur_level].grid) * scale;
	Square square = {};
	int x = int(int(mouse.x - grid_sp.getPosition().x) / distance);
	int y = int(int(mouse.y - grid_pos.y) / distance);
	if (x < 0 || x >= levels[cur_level].grid) return {-10,false,EMPTY};
	if (y < 0 || y >= levels[cur_level].grid) return { -10,false,EMPTY };
	square.index = (y * levels[cur_level].grid) + x;
	square.isFilled = (levels[cur_level].level_map[y][x] != EMPTY);
	square.color = levels[cur_level].level_map[y][x];
	return square;
}
std::vector<Square> getAllSquares() {
	Level level = levels[cur_level];
	const int square_count = level.grid * level.grid;
	std::vector<Square> squares;
	for (int i = 0; i < square_count; i++) {
		Square square;
		square.index = i;
		int x = i % level.grid;
		int y = (i - x) / level.grid;
		square.isFilled = (level.level_map[y][x] != EMPTY);
		square.color = level.level_map[y][x];
		squares.push_back(square);
	}
	return squares;
}
sf::Vector2f getCenter(int index,int grid) {
	int x = (index) % grid;
	int y = (index - x) / grid;
	sf::Vector2f square = sf::Vector2f((grid_sp.getPosition().x + x * distance),(grid_sp.getPosition().y + y*distance));
	return sf::Vector2f(square.x + (distance / 2), square.y + (distance / 2));
}
bool isValidLink(int start, int dest) {
	Level level = levels[cur_level];
	int start_x = start % level.grid;
	int start_y = (start - start_x) / level.grid;

	int dest_x = dest % level.grid;
	int dest_y = (dest - dest_x) / level.grid;

	if (!(abs(dest - start) == 7 || abs(dest - start) == 1)&&(start_x == dest_x || start_y == dest_y)) {
		return false;
	}

	/*if (getSquare(dest).isFilled) {
		return false;
	}*/
	return true;
}
void  draw_link(int start, int destination,GridData color,Level level) {
	if (!isValidLink(start, destination)) 
		return;
	sf::Vector2f start_pos = getCenter(start,level.grid);
	sf::Vector2f destination_pos = getCenter(destination,level.grid);

	int start_x = start % level.grid;
	int start_y = (start - start_x) / level.grid;

	int dest_x = destination % level.grid;
	int dest_y = (destination - dest_x) / level.grid;

	

	sf::Color col = to_color(color);
	sf::RectangleShape rect_link;
	rect_link.setFillColor(col);
	int thickness = 10, longness = (grid.getSize().x / level.grid)*scale;
	float link_width=0, link_height=0;
	
	//Vertical
	if (start_x == dest_x) {
		link_height = longness;
		link_width = thickness;
		rect_link.setPosition(start_pos.x - thickness / 2, start_pos.y);
		if (start_y > dest_y) {
			link_height = -link_height;
		}
	}
	//Horizontal
	else if (start_y == dest_y) {
		link_height = thickness;
		link_width = longness;
		rect_link.setPosition(start_pos.x, start_pos.y - thickness / 2);
		if (start_x > dest_x) {
			link_width = -link_width;
		}
	}
	else {
		//std::cout << "Invalid indices\n";
		return;
	}
	rect_link.setSize(sf::Vector2f(link_width, link_height));
	window.draw(rect_link);
}
int getSelected(Input input) {
	Level level = levels[cur_level];
	int i = 0;
	Square selected_sq = getCurrentSquare();
	selected = selected_sq.index;
	if (selected_sq.isFilled)return selected_sq.index;
	return -10;
}
void draw_grid(int num) {
	float xcenter=0,ycenter=0;
	std::string path = "Assets/" + std::to_string(num) + "x" + std::to_string(num) + ".png";
	if (grid.loadFromFile(path)) {
		xcenter = (grid.getSize().x / 2) * scale;
		ycenter = (grid.getSize().y / 2) * scale;
	}
	else {
		std::cout << "Invalid number in draw_grid\n";
		return;
	}
	grid_sp.setTexture(grid);
	grid_sp.setScale(scale,scale);
	grid_sp.setPosition(((window.getSize().x / 2) - xcenter), ((window.getSize().y / 2) - ycenter));
	window.draw(grid_sp);
}

void draw_level(Level &level) {
	//draw_grid(level.grid);
	radius = 20;
	distance = (grid.getSize().x / level.grid) * scale;
	int additional = distance/2 - radius;
	float cur_y = grid_sp.getPosition().y+additional;
	for (int i = 0; i < level.grid; i++) {
		float cur_x = grid_sp.getPosition().x+additional;
		for (int j = 0; j < level.grid; j++) {
			if(level.level_map[i][j] != EMPTY){
				sf::CircleShape circle;
				sf::Color ball_color = to_color(level.level_map[i][j]);
				circle.setFillColor(ball_color);
				circle.setRadius(radius);
				circle.setPosition(cur_x, cur_y);
				int index = ((i-1)*level.grid) + j;
				Ball ball = { circle,j,i,index};
				level.circles.push_back(ball);   
			}
			cur_x += distance;
		}
		cur_y += distance;
	}
	for (auto Ball : level.circles) {
		window.draw(Ball.circle);
	}
}
void simulate_game(Input input) {
	mouse = (sf::Mouse::getPosition() - window.getPosition() - sf::Vector2i(7, 30));
	cur_level = 0;
	updateLevels();
	draw_level(levels[cur_level]);
	draw_grid(levels[cur_level].grid);
	if (is_down(LEFT_CLICK)) {
		if (getSelected(input) != -10) {
			if (!isSelected) {
				isSelected = true;
				starting_square = getSquare(selected);
				if (starting_square.isFilled) {
					current_color = starting_square.color;
				}
				else {
					starting_square = { -10,0,EMPTY };
				}
			}
		}
	}
	if (released(LEFT_CLICK) && isSelected) {
		isSelected = false;
		selected = -10;
		starting_square = {-10,0,EMPTY};
	}
	if (isSelected) {
		Square current_square = getCurrentSquare();
		bool isCompletedSquare = false;
		for (auto square : completed_squares) {
			if (starting_square.index == square.index) {
				isCompletedSquare = true;
				break;
			}
		}
		if (current_square.index != prev_square.index && !isCompletedSquare && isValidLink(prev_square.index, current_square.index)) {
			int index = getSelected(input);
			if ((!current_square.isFilled || (current_square.color == current_color)) && prev_square.index != -10) {
				Link link = { prev_square,current_square };
				links.push_back(link);
				if (current_square.isFilled && current_square.color == current_color) {
					if (current_square.index != starting_square.index) {
						completed_squares.push_back(current_square);
						completed_squares.push_back(starting_square);
						starting_square = {-10,0,EMPTY};
						LinkThread linkthread;
						linkthread.links = links;
						linkthread.color = current_color;
						completed_links.push_back(linkthread);
						
						links = {};
					}
					else {
						links = {};
					}
				}
				
			}
			else {
				links = {};
				starting_square = { -10,0,EMPTY };
			}
			prev_square = current_square;
		}
	}
	else {
		links = {};
	}
	for (auto link_thread : completed_links) {
		for (auto link : link_thread.links) {
			draw_link(link.first.index, link.second.index, link_thread.color, levels[cur_level]);
		}
	}
	for (auto link : links) {
		draw_link(link.first.index,link.second.index,current_color,levels[cur_level]);
	}
	if (pressed(BUTTON_A)) {
		system("cls");
		completed_links = {};
		completed_squares = {};
	}
}
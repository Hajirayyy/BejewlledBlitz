#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>

using namespace std;
using namespace sf;

bool enteringName = false;
const int MaxNameLength = 50;

char playerName[MaxNameLength]; // Array to store player name
int nameIndex = 0; // Index to keep track of characters entered

const int gridSize = 8;  //size of matrix

//varibles to set grid on window
const int cellSize = 50;
const int spacing = 5;
const int numJewels = 5;
//variable to sum up the total score 
int score = 0;
int highscore = 0;
int tempscore = 0;
//2D array
int grid[gridSize][gridSize];

//all the functions that are called //

//initializing the grid and passing windows in various functions
void initializeGrid();
void WelcomeWindow(RenderWindow& window);
void Window1(RenderWindow& window);
void drawGrid(RenderWindow& window, float gridx, float gridy, Texture textures[]);
void Window2(RenderWindow& window);
void endwindow(int score, RenderWindow& window);

//the funcs that check weather the selected jewel can be swapped with the designated slot or not
bool canSwapHorizontal(int i, int j, int grid[8][8]);
bool canSwapVertical(int i, int j, int grid[8][8]);
bool canSwap(int i1, int j1, int i2, int j2, int grid[8][8]);

//funcs for when 3 , 4 , 5 jewels are togather
int horizontal3(int& score);
int verticle3(int& score);
int verticle4(int& score);
int horizontal4(int& score);    //the score is passed by reference so we keep track whenever score is increased by explosion

//special gems check
int destroyerGem(int& score);
bool isElbow(int i1, int j1, int i2, int j2, int i3, int j3, int i4, int j4, int i5, int j5);

//special gems explosion functions
void flameDestruction(int i, int j);
void destroy(int i, int j);



//main body
int main()
{
    RenderWindow window;
    window.create(VideoMode(1280, 800), "Bejeweled Blitz");  //creating window

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)

                window.close();

        }
        window.clear(Color::White);
        WelcomeWindow(window);  //calling the 1st function for window for user to select Play Or Exit
    }

    return 0;
}

//1st window to be displayed//
void WelcomeWindow(RenderWindow& window)
{
    SoundBuffer GameSoundBuffer;
    if (!GameSoundBuffer.loadFromFile("Sound/gameSound.mp3"))
    {
        cout << "error loading sound";
    }
    Sound gameSound;
    gameSound.setBuffer(GameSoundBuffer);

    gameSound.play();

    // Load background image
    Texture FirstBackgroundTexture;
    if (!FirstBackgroundTexture.loadFromFile("Welcome/IMGG.jpg")) //file path
    {
        cout << "Failed to load background image!" << endl;
        return;
    }

    FirstBackgroundTexture.setSmooth(true);  //avoiding blurry image

    // Load font for text
    Font font;
    if (!font.loadFromFile("Font/Comica Boom.otf"))
    {
        cout << "Error loading font!" << endl;  //error handling
        return;
    }

    // Create background sprite and set its texture
    Sprite background(FirstBackgroundTexture);

    // Title text setup
    Text text;
    text.setString("BEJEWELLED BLITZ");
    text.setCharacterSize(60);
    text.setStyle(Text::Bold);
    text.setFont(font);
    text.setFillColor(Color::Magenta);
    text.setPosition(380, 300);

    // 'Play' option text setup
    Text text2;
    text2.setString("Play");
    text2.setCharacterSize(40);
    text2.setStyle(Text::Bold);
    text2.setFont(font);
    text2.setFillColor(Color::Green);
    text2.setPosition(200, 500);

    // 'Exit' option text setup
    Text text3;
    text3.setString("Exit");
    text3.setCharacterSize(40);
    text3.setStyle(Text::Bold);
    text3.setFont(font);
    text3.setFillColor(Color::Red);
    text3.setPosition(970, 500);

    //Credit names
    Text creditsName("Hajirah & Noor-ul-Sabah", font, 20);
    creditsName.setFillColor(Color::White);
    creditsName.setPosition(500.f, 700.f);

    //Credit roll numbers
    Text creditRollNumber("23L-0929 & 23L0915", font, 15);
    creditRollNumber.setFillColor(Color::White);
    creditRollNumber.setPosition(550.f, 725.f);

    bool breakLoop = false; //var to check if user enters P we should the next window otherwise terminate 

    // Event loop to handle user interaction
    while (window.isOpen() && !breakLoop)
    {
        Event event;
        while (window.pollEvent(event))
        {
            // Check for window closure
            if (event.type == Event::Closed)
            {
                window.close();
                breakLoop = true;
            }

            // Check for key presses
            if (event.type == Event::KeyPressed)
            {
                // Start game on 'P' key press
                if (event.key.code == Keyboard::P)
                {
                    // Call functions to start game windows
                    Window1(window);
                    Window2(window);  //calling out next windows to be displayed

                    // Break the loop and signal to enter name
                    breakLoop = true;
                    enteringName = true;
                    break;
                }
                // Close window on 'E' key press
                else if (event.key.code == Keyboard::E)
                {
                    window.close();
                    breakLoop = true;
                    break;
                }
            }
        }
        window.clear();

        // Draw elements to the window
        window.draw(background);
        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.draw(creditsName);
        window.draw(creditRollNumber);

        window.display(); // Display the window
    }
}


// Function to make next window to give instructions for playerand name entry
void Window1(RenderWindow& window)
{
    // Load background image
    Texture texture;
    if (!texture.loadFromFile("window1/image.jpg"))
    {
        cout << "Failed to load background image!" << endl;
        return;
    }

    // Create background sprite
    Sprite background(texture);

    // Load font for text
    Font font;
    if (!font.loadFromFile("Font/RetroGaming.ttf"))
    {
        cout << "Failed to load font!" << endl;
        return;
    }

    // Text setup for greeting
    Text text("Hello there!", font, 70);
    text.setFillColor(Color::White);
    text.setStyle(Text::Regular);
    text.setPosition(200.f, 100.f);

    // Instruction text setup
    Text instruction("Hope you are having a good day.\nUse keyboard arrows to move cursor.\nPress Escape key to select a jewel.\nPress Enter key to swap.\nGOOD LUCK BUDDY!", font, 40);
    instruction.setFillColor(Color::White);
    instruction.setStyle(Text::Regular);
    instruction.setPosition(400.f, 200.f);

    // Text setup for entering name prompt
    Text text1("Enter Your Name:\n", font, 50);
    text1.setFillColor(Color::Magenta);
    text1.setStyle(Text::Regular);
    text1.setPosition(320.f, 500.f);


    // Text setup for displaying entered name
    Text inputDisplay("", font, 50);
    inputDisplay.setFillColor(Color::Magenta);
    inputDisplay.setStyle(Text::Regular);
    inputDisplay.setPosition(700.f, 500.f);

    // Event loop to handle user input
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            // Close window on 'X' click
            if (event.type == Event::Closed)
                window.close();

            // Handle text input
            if (event.type == Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    // Check for backspace and limit character count
                    if (event.text.unicode == '\b' && nameIndex > 0)
                    {
                        playerName[--nameIndex] = '\0';
                    }
                    else if (event.text.unicode != '\b' && nameIndex < MaxNameLength - 1)
                    {
                        playerName[nameIndex++] = static_cast<char>(event.text.unicode);
                        playerName[nameIndex] = '\0';
                    }

                    // Update displayed text
                    inputDisplay.setString(playerName);
                }
            }

            // Save name to file on 'Enter' key press
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
            {
                // Write entered name to file
                ofstream outputFile("name.txt");
                if (outputFile.is_open())
                {
                    outputFile << playerName;
                    outputFile.close();
                    cout << "Name saved to file." << endl;

                    enteringName = false; // Exit name entry loop
                }
                else
                {
                    cout << "Unable to open file for writing." << endl;  //error handling
                }

                window.clear();
                return;
            }
        }

        window.clear();

        // Draw elements to the window
        window.draw(background);
        window.draw(text);
        window.draw(instruction);
        window.draw(text1);
        window.draw(inputDisplay);

        window.display(); // Display the window
    }
}


// Function to handle the game window where gameplay occurs
void Window2(RenderWindow& window)
{
    ifstream scoreFileRead("highscore.txt");
    scoreFileRead >> highscore;
    score = 0;

    // Seed the random number generator using the current time
    srand(time(0));

    // Load the background image for the game window
    Texture gameBackground;
    if (!gameBackground.loadFromFile("window2/gameBack.jpg"))
    {
        cout << "Failed to load background image!" << endl;

        return;
    }

    // Create a sprite for the background image
    Sprite backgrounds;
    backgrounds.setTexture(gameBackground);

    // Array to hold textures for different jewel types
    Texture textures[15];

    // calling the func to Initialize the game table
    initializeGrid();

    // Constants defining the grid position 
    const float gridX = 700.0f;
    const float gridY = 120.0f;

    // Variables to track cursor and selected jewel positions//
//when jewel not selected 
    int cursorPositionX = 0;
    int cursorPositionY = 0; //represent the X and Y coordinates of the cursor in a grid
    //when jewel is slected 
    int selectedX = 0;
    int selectedY = 0;

    int selectedJewelType = -1; //this varible tells which jewel position is selcted 

    bool jewelselected = false; //this variable checks if user have used "enter" to select the jewel or not 
    //default value -1

// Create a rectangle shape for the cursor
    RectangleShape cursor(Vector2f(cellSize, cellSize)); //the cell size of jewwel is used to fix the length and width of cursor so it wont cover surrounding jewels 
    cursor.setFillColor(Color::Transparent);
    cursor.setOutlineThickness(4);
    cursor.setOutlineColor(Color::White);
    cursor.setPosition(gridX, gridY);  //these varibles specify and update the position of cursor

    //  TIMER for the game //

    Clock timer;
    Time elapsedTime;
    Time timeLimit = seconds(60);  //time limit 
    Text timeText;

    // Font setup for displaying time in the window
    Font font;
    if (!font.loadFromFile("Font/arial_bold.ttf"))
    {
        cout << "Error loading font!" << endl;
        return;
    }

    // Initialize text for displaying time //
// Time text setup
    timeText.setFont(font);
    timeText.setString("Time");
    timeText.setCharacterSize(40);
    timeText.setPosition(100, 400);
    timeText.setFillColor(Color::Magenta);

    Text time;

    // Display the time remaining
    time.setFont(font);
    time.setCharacterSize(40);
    time.setPosition(100, 480);
    time.setFillColor(Color::White);

    //Initialize text for displaying score//
// Text setup for displaying score
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score");
    scoreText.setCharacterSize(40);
    scoreText.setPosition(100, 100);
    scoreText.setFillColor(Color::Magenta);

    Text scoreDisplay;
    Text highscoreDisplay;
    //HIGH SCORE
    Text highscoreText;
    highscoreText.setFont(font);
    highscoreText.setString("High Score");
    highscoreText.setCharacterSize(40);
    highscoreText.setPosition(100, 300);
    scoreText.setFillColor(Color::Magenta);

    // Display the player's score
    scoreDisplay.setFont(font);
    scoreDisplay.setCharacterSize(40);
    scoreDisplay.setPosition(100, 180);
    scoreDisplay.setFillColor(Color::White);

    highscoreDisplay.setFont(font);
    highscoreDisplay.setCharacterSize(40);
    highscoreDisplay.setPosition(100, 380);
    highscoreDisplay.setFillColor(Color::White);

    // Load sound effect for jewel swapping
    SoundBuffer swapBuffer;
    if (!swapBuffer.loadFromFile("Sound/swapSound.wav"))
    {
        cout << "Error loading sound";
        return;
    }
    Sound swapSound;
    swapSound.setBuffer(swapBuffer);

    // Game loop: Continuously runs while the game window is open
    while (window.isOpen()) {
        Event event;
        // Event handling loop: Processes events of key presses
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            // Check for keystrokes when a jewel is not selected
            if (event.type == Event::KeyPressed && !jewelselected)
            {
                // Move the cursor within the grid boundaries using arrow keys

                if (event.key.code == Keyboard::Up && cursorPositionY > 0)
                    cursorPositionY--;   // Update cursor position based on key inputs
                else if (event.key.code == Keyboard::Down && cursorPositionY < gridSize - 1)
                    cursorPositionY++;
                else if (event.key.code == Keyboard::Left && cursorPositionX > 0)
                    cursorPositionX--;
                else if (event.key.code == Keyboard::Right && cursorPositionX < gridSize - 1)
                    cursorPositionX++;

                //when ENTER is pressed , the jewel gets selected and the position of cursor remians fixed till further actions
                else if (event.key.code == Keyboard::Enter) {
                    selectedX = cursorPositionX;
                    selectedY = cursorPositionY;
                    selectedJewelType = grid[selectedY][selectedX];
                    jewelselected = true;
                    break;
                }
                // Update visual position of the cursor
                cursorPositionX = max(0, min(cursorPositionX, gridSize - 1));
                //min(cursorPositionX, gridSize - 1) : ensures that cursorPositionX doesn't exceed the maximum value of "gridSize - 1"
                //max(0, ...) : ensures that cursorPositionX doesn't go below 0
                cursorPositionY = max(0, min(cursorPositionY, gridSize - 1));

                cursor.setPosition(gridX + cursorPositionX * (cellSize + spacing), gridY + cursorPositionY * (cellSize + spacing));
                // 1st part calculates the X-coordinate for the cursor  
//  It starts from the grid's initial X position (gridX) and adds the offset based on the cursorPositionX.
                                                                 // this calculates the Y-coordinate for the cursor 
                                                                 //adds the total vertical offset based on the cursor's Y position within the grid to gridY
            }
            // Check for keystrokes when a jewel is selected
            else if (event.type == Event::KeyPressed && jewelselected)
            {
                if (event.key.code == Keyboard::W && selectedY == 0 || event.key.code == Keyboard::S && selectedY == gridSize - 1 || event.key.code == Keyboard::A && selectedX == 0 || event.key.code == Keyboard::D && selectedX == gridSize - 1)
                    jewelselected = false;

                // Handle swapping the selected jewel with adjacent keyboard keys.
                else if (event.key.code == Keyboard::Up)     //W for UP.
                {
                    if (canSwap(selectedY, selectedX, selectedY - 1, selectedX, grid))
                    {
                        swap(grid[selectedY][selectedX], grid[selectedY - 1][selectedX]);
                        swapSound.play();
                        selectedY--;
                    }
                    jewelselected = false;
                }
                else if (event.key.code == Keyboard::Down) //S for Down.
                {
                    if (canSwap(selectedY, selectedX, selectedY + 1, selectedX, grid))
                    {
                        swap(grid[selectedY][selectedX], grid[selectedY + 1][selectedX]);
                        swapSound.play();
                        selectedY++;
                    }
                    jewelselected = false;
                }
                else if (event.key.code == Keyboard::Left)  //A for LEFT.
                {
                    if (canSwap(selectedY, selectedX, selectedY, selectedX - 1, grid))
                    {
                        swap(grid[selectedY][selectedX], grid[selectedY][selectedX - 1]);
                        swapSound.play();
                        selectedX--;
                    }
                    jewelselected = false;
                }
                else if (event.key.code == Keyboard::Right)   //D for Right.
                {
                    if (canSwap(selectedY, selectedX, selectedY, selectedX + 1, grid))
                    {
                        swap(grid[selectedY][selectedX], grid[selectedY][selectedX + 1]);
                        swapSound.play();
                        selectedX++;
                    }
                    jewelselected = false;
                }
            }
        }
        // Update the grid after a successful swap

        //timer
        elapsedTime = timer.getElapsedTime();
        if (elapsedTime >= timeLimit)


            endwindow(score, window);

        int timeLeft = 60 - elapsedTime.asSeconds();
        time.setString(to_string(timeLeft));

        highscoreText.setString("High Score : " + to_string(highscore));
        window.clear(Color::Black);

        //All explosion functions are called in a sequence according to their functionality
        drawGrid(window, gridX, gridY, textures);
        destroyerGem(score);
        drawGrid(window, gridX, gridY, textures);
        verticle4(score);
        drawGrid(window, gridX, gridY, textures);
        horizontal4(score);
        drawGrid(window, gridX, gridY, textures);
        verticle3(score);
        drawGrid(window, gridX, gridY, textures);
        horizontal3(score);
        drawGrid(window, gridX, gridY, textures);

        if (score > highscore)
        {
            tempscore = score;
            ofstream outfile("highscore.txt");
            outfile << tempscore;
        }
        window.clear();
        window.draw(backgrounds);
        drawGrid(window, gridX, gridY, textures);
        window.draw(cursor);

        //The jewel images are uploaed.
        for (int i = 0; i < 15; i++) {
            string filename = "gemimages/SHAPE " + to_string(i + 1) + ".png";
            if (!textures[i].loadFromFile(filename)) {
                cout << "Failed to load image: " << filename << endl;
            }
            textures[i].setSmooth(true);
        }

        if (jewelselected)
        {
            //if a jewel is selected, then position of cursor is managed.
            if (selectedJewelType != -1)
            {
                Sprite selectedjewel; //to represent the selected jewel visually

                selectedjewel.setTexture(textures[selectedJewelType]); // likely holds an index or identifier for the type of jewel selected

                // calculate the scale factors for the selected jewel's texture
                float scalefactorx = static_cast<float>(cellSize) / textures[selectedJewelType].getSize().x;
                float scalefactory = static_cast<float>(cellSize) / textures[selectedJewelType].getSize().y;

                selectedjewel.setScale(scalefactorx, scalefactory);

                //calculation positions the sprite according to the grid layout
                selectedjewel.setPosition(gridX + selectedX * (cellSize + spacing), gridY + selectedY * (cellSize + spacing));
                //draw
                window.draw(selectedjewel);
            }
        }

        scoreDisplay.setString(to_string(score));
        window.draw(timeText);
        window.draw(time);
        window.draw(scoreText);
        window.draw(highscoreText);
        window.draw(scoreDisplay);
        window.draw(highscoreDisplay);

        window.display();
    }

}

void endwindow(int score, RenderWindow& window) {
    ifstream inputFile("name.txt"); // Read from input file

    char name[256];
    if (inputFile.is_open()) {
        inputFile.getline(name, 256);
        inputFile.close();
    }
    else {
        cout << "Unable to open file name.txt";
        return;
    }

    //texture of background
    Texture FirstBackgroundTexture;

    if (!FirstBackgroundTexture.loadFromFile("window/imgg.jpg"))
    {
        cout << "Failed to load background image!" << endl;
        return;
    }

    FirstBackgroundTexture.setSmooth(true);

    Font font;   //font is loaded
    if (!font.loadFromFile("Font/Comica Boom.otf"))
    {
        cout << "error loading font!" << endl;
        return;
    }

    Sprite background(FirstBackgroundTexture);

    //all necessary texts shown in end window.
    Text text;
    text.setString("GAME OVER");
    text.setCharacterSize(60);
    text.setStyle(Text::Bold);
    text.setFont(font);
    text.setFillColor(Color::Blue);
    text.setPosition(465, 300);

    Text Name;
    Name.setString("Name: ");
    Name.setCharacterSize(30);
    Name.setStyle(Text::Regular);
    Name.setFont(font);
    Name.setFillColor(Color::White);
    Name.setPosition(500.f, 500.f);


    Text nameText(name, font, 30);
    nameText.setFillColor(Color::White);
    nameText.setPosition(620.f, 500.f);

    Text scoreText("Score:      " + to_string(score), font, 30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(500.f, 560.f);

    Text creditsName("Hajirah & Noor-ul-Sabah", font, 20);
    creditsName.setFillColor(Color::White);
    creditsName.setPosition(500.f, 700.f);

    Text creditRollNumber("23L-0929 & 23L0915", font, 15);
    creditRollNumber.setFillColor(Color::White);
    creditRollNumber.setPosition(550.f, 725.f);


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(background);
        window.draw(Name);
        window.draw(text);
        window.draw(nameText);
        window.draw(scoreText);
        window.draw(creditsName);
        window.draw(creditRollNumber);
        window.display();
    }
}

void initializeGrid() {
    srand(time(0));

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            // Initialize each grid cell with a random jewel type
            grid[i][j] = rand() % numJewels + 1;

            int jewelType = grid[i][j];

            switch (jewelType) {
            case 1:
                // Assign texture 1 to grid[i][j]
                grid[i][j] = 0; // Index 0 in textures griday corresponds to texture 1
                break;
            case 2:
                // Assign texture 2 to grid[i][j]
                grid[i][j] = 1; // Index 1 in textures griday corresponds to texture 2
                break;
            case 3:
                // Assign texture 3 to grid[i][j]
                grid[i][j] = 2; // Index 2 in textures griday corresponds to texture 3
                break;
            case 4:
                // Assign texture 4 to grid[i][j]
                grid[i][j] = 3; // Index 3 in textures griday corresponds to texture 4
                break;
            case 5:
                // Assign texture 5 to grid[i][j]
                grid[i][j] = 4; // Index 4 in textures griday corresponds to texture 5
                break;
            case 6:
                // Assign texture 6 to grid[i][j]
                grid[i][j] = 5; // Index 5 in textures griday corresponds to texture 6
                break;
            case 7:
                // Assign texture 7 to grid[i][j]
                grid[i][j] = 6; // Index 6 in textures griday corresponds to texture 7
                break;
            case 8:
                // Assign texture 8 to grid[i][j]
                grid[i][j] = 7; // Index 7 in textures griday corresponds to texture 8
                break;
            case 9:
                // Assign texture 9 to grid[i][j]
                grid[i][j] = 8; // Index 8 in textures griday corresponds to texture 9
                break;
            case 10:
                // Assign texture 10 to grid[i][j]
                grid[i][j] = 9; // Index 9 in textures griday corresponds to texture 10
                break;
            case 11:
                // Assign texture 10 to grid[i][j]
                grid[i][j] = 10; // Index 10 in textures griday corresponds to texture 11
                break;
            case 12:
                // Assign texture 10 to grid[i][j]
                grid[i][j] = 11; // Index 11 in textures griday corresponds to texture 12
                break;
            case 13:
                // Assign texture 10 to grid[i][j]
                grid[i][j] = 12; // Index 12 in textures griday corresponds to texture 13
                break;
            case 14:
                // Assign texture 10 to grid[i][j]
                grid[i][j] = 13; // Index 13 in textures griday corresponds to texture 14
                break;
            case 15:
                // Assign texture 10 to grid[i][j]
                grid[i][j] = 14; // Index 14 in textures griday corresponds to texture 15
                break;
            }

        }
    }
}

//this function is called everytime any change in grid occurs
void drawGrid(RenderWindow& window, float gridx, float gridy, Texture textures[])
{
    //it draws grid in the window, and update the position of jewels
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j) //nested loop
        {
            if (grid[i][j] != -1)
            {

                Vector2u texturesize = textures[grid[i][j]].getSize();
                float scalefactorx = static_cast<float>(cellSize) / texturesize.x;
                float scalefactory = static_cast<float>(cellSize) / texturesize.y;

                Sprite jewel;
                jewel.setTexture(textures[grid[i][j]]);
                jewel.setScale(scalefactorx, scalefactory);

                jewel.setPosition(gridx + j * (cellSize + spacing), gridy + i * (cellSize + spacing));

                window.draw(jewel);
            }
        }
    }
}

bool canSwapHorizontal(int i, int j, int grid[8][8])
{
    //function finds out if swapping is legal horizontally
    bool flag = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            if ((grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2]) ||
                (grid[i][j] - 5 == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2]) ||
                (grid[i][j] == grid[i][j + 1] - 5 && grid[i][j + 1] - 5 == grid[i][j + 2]) ||
                (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] - 5) ||
                (grid[i][j] - 10 == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2]) ||
                (grid[i][j] == grid[i][j + 1] - 10 && grid[i][j + 1] - 10 == grid[i][j + 2]) ||
                (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] - 10) ||
                (grid[i][j] > 5 && (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] + 5)) ||
                (grid[i][j] > 5 && (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] + 5 == grid[i][j + 2] + 5))) {
                flag = true;
            }
        }
    }
    return flag;
}

bool canSwapVertical(int i, int j, int grid[8][8])
{
    //function finds out if swapping is legal vertically
    bool flag = false;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if ((grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j]) ||
                (grid[i][j] - 5 == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j]) ||
                (grid[i][j] == grid[i + 1][j] - 5 && grid[i + 1][j] - 5 == grid[i + 2][j]) ||
                (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] - 5) ||
                (grid[i][j] - 10 == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j]) ||
                (grid[i][j] == grid[i + 1][j] - 10 && grid[i + 1][j] - 10 == grid[i + 2][j]) ||
                (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] - 10) ||
                (grid[i][j] > 5 && (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] + 5)) ||
                (grid[i][j] > 5 && (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] + 5 == grid[i][j + 2] + 5))) {
                flag = true;
            }
        }
    }
    return flag;
}

bool canSwap(int i1, int j1, int i2, int j2, int grid[8][8])
{
    //the function combines the result from above 2 functions.
    swap(grid[i1][j1], grid[i2][j2]);

    bool validSwap = canSwapHorizontal(i1, j1, grid) || canSwapVertical(i1, j1, grid) ||
        canSwapHorizontal(i2, j2, grid) || canSwapVertical(i2, j2, grid);

    swap(grid[i1][j1], grid[i2][j2]);

    return validSwap;
}

int verticle3(int& score)
{
    //when 3gems are found verticle(up & down), this function works,
    for (int i = 0; i < gridSize - 2; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j])    //for simple swap
            {

                grid[i][j] = rand() % numJewels;
                grid[i + 1][j] = rand() % numJewels;
                grid[i + 2][j] = rand() % numJewels;
                score += 30;
                return score;

            }
            if (grid[i][j] - 5 == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j])    //for flame swap
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] - 5 && grid[i + 1][j] - 5 == grid[i + 2][j])
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] - 5)
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }

            if (grid[i][j] - 10 == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j])   //for destroyer swap
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] - 10 && grid[i + 1][j] - 10 == grid[i + 2][j])
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] - 10)
            {
                destroy(i, j);
                score += 150;
                return score;
            }

        }
    }

    return score;
}

int horizontal3(int& score)
{
    //when 3 gems are found horizontally(left & right) this function is called.
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize - 2; j++)
        {
            if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2]) // for simple 3 swaps
            {
                grid[i][j] = rand() % numJewels;
                grid[i][j + 1] = rand() % numJewels;
                grid[i][j + 2] = rand() % numJewels;
                score += 30;
                return score;
            }
            if (grid[i][j] - 5 == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2]) //for flame swaps
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] - 5 && grid[i][j + 1] - 5 == grid[i][j + 2])
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] - 5)
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }

            if (grid[i][j] - 10 == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2])  //for destroyer swaps
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] - 10 && grid[i][j + 1] - 10 == grid[i][j + 2])
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] - 10)
            {
                destroy(i, j);
                score += 150;
                return score;
            }

        }
    }
    return score;
}

void flameDestruction(int i, int j) //function that explodes flame gem in 3by3 radius
{
    grid[i + 1][j] = rand() % numJewels;
    grid[i + 2][j] = rand() % numJewels;
    grid[i - 1][j - 1] = rand() % numJewels;
    grid[i - 1][j] = rand() % numJewels;
    grid[i - 1][j + 1] = rand() % numJewels;
    grid[i + 1][j - 1] = rand() % numJewels;
    grid[i + 1][j] = rand() % numJewels;
    grid[i + 1][j + 1] = rand() % numJewels;
}

int verticle4(int& score)
{
    //conditions that control what happen when 4 same gems meet.
    for (int i = 0; i < gridSize - 3; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j]) //conditions that check for flame formation.
            {
                grid[i][j] = rand() % numJewels;
                grid[i + 1][j] = rand() % numJewels;
                grid[i + 2][j] = rand() % numJewels;
                grid[i + 3][j] += 5;
                score += 40;
                return score;
            }
            if (grid[i][j] - 5 == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j])//for flame explosion
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] - 5 && grid[i + 1][j] - 5 == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j])
            {
                {
                    flameDestruction(i, j);
                    score += 100;
                    return score;
                }
            }
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] - 5 && grid[i + 2][j] - 5 == grid[i + 3][j])
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j] - 5)
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }

            if (grid[i][j] - 10 == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j])  // for destroyer explosion
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] - 10 && grid[i + 1][j] - 10 == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j])
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] - 10 && grid[i + 2][j] - 10 == grid[i + 3][j])
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] && grid[i + 2][j] == grid[i + 3][j] - 10)
            {
                destroy(i, j);
                score += 150;
                return score;
            }
        }
    }
    return score;
}

int horizontal4(int& score)
{
    //condition for horizontal swaps of 4. and the formation of flame gem.
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize - 5; j++)
        {
            if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3]) // for flame formation
            {

                grid[i][j] = rand() % numJewels;
                grid[i][j + 1] = rand() % numJewels;
                grid[i][j + 2] = rand() % numJewels;
                grid[i][j + 3] += 5;
                score += 40;
                return score;
            }
            if (grid[i][j] - 5 == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3])  //for flame destruction
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] - 5 && grid[i][j + 1] - 5 == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3])
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] - 5 && grid[i][j + 2] - 5 == grid[i][j + 3])
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3] - 5)
            {
                flameDestruction(i, j);
                score += 100;
                return score;
            }

            if (grid[i][j] - 10 == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3])  // for destroyer destruction
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] - 10 && grid[i][j + 1] - 10 == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3])
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] - 10 && grid[i][j + 2] - 10 == grid[i][j + 3])
            {
                destroy(i, j);
                score += 150;
                return score;
            }
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i][j + 3] - 10)
            {
                destroy(i, j);
                score += 150;
                return score;
            }
        }
    }
    return score;
}

bool isElbow(int i1, int j1, int i2, int j2, int i3, int j3, int i4, int j4, int i5, int j5)
{
    //function that checks if elbow is formed
    bool elbowFound = false;
    if (i1 >= 0 && i1 < gridSize && j1 >= 0 && j1 < gridSize &&
        i2 >= 0 && i2 < gridSize && j2 >= 0 && j2 < gridSize &&
        i3 >= 0 && i3 < gridSize && j3 >= 0 && j3 < gridSize &&
        i4 >= 0 && i4 < gridSize && j4 >= 0 && j4 < gridSize &&
        i5 >= 0 && i5 < gridSize && j5 >= 0 && j5 < gridSize && grid[i1][j1] == grid[i2][j2] &&
        grid[i2][j2] == grid[i3][j3] && grid[i3][j3] == grid[i4][j4] && grid[i4][j4] == grid[i5][j5])
    {
        //variable returns value in bool
        elbowFound = true;
    }
    return elbowFound;
}

void destroy(int i, int j)
{
    //function that explodes the row and col for destroyer gem
    for (int row = 0; row < gridSize; row++)
    {
        grid[row][j] = rand() % numJewels;
    }
    for (int col = 0; col < gridSize; col++)
    {
        grid[i][col] = rand() % numJewels;
    }
}

int destroyerGem(int& score)
{
    //funcction that checks for elbow condition and destroyer formation
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            //////////FOR LEFT-UP////////////
//case 1 when 3 gems are horizontal and 2 are verticle
            if (j >= 3 && grid[i][j] == grid[i][j - 1] && grid[i][j - 1] == grid[i - 1][j - 1] && grid[i - 1][j - 1] == grid[i - 2][j - 1] && grid[i - 2][j - 1] == grid[i - 3][j - 1]) {
                bool isElbowResult = isElbow(i, j, i, j - 1, i - 1, j - 1, i - 2, j - 1, i - 3, j - 1);
                if (isElbowResult) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j - 1] = rand() % numJewels;
                    grid[i - 1][j - 1] = rand() % numJewels;
                    grid[i - 2][j - 1] = rand() % numJewels;
                    grid[i - 3][j - 1] = rand() % numJewels;
                }
                return score;
            }

            //case2 when 4 gems are horizontal and 1 is verticle
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i - 1][j + 2] && grid[i - 1][j + 2] == grid[i - 2][j + 2])
            {
                isElbow(i, j, i, j + 1, i, j + 2, i - 1, j + 2, i - 2, j + 2);
                if (isElbow) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j + 1] = rand() % numJewels;
                    grid[i][j + 2] = rand() % numJewels;
                    grid[i - 1][j + 2] = rand() % numJewels;
                    grid[i - 2][j + 2] = rand() % numJewels;
                }
                return score;
            }
            //case3 when 1 gem is horizontal and 4 are verticle
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 1][j - 1] && grid[i + 1][j - 1] == grid[i + 1][j - 2] && grid[i + 1][j - 2] == grid[i + 1][j - 3]) {
                bool isElbowResult = isElbow(i, j, i + 1, j, i + 1, j - 1, i + 1, j - 2, i + 1, j - 3);
                if (isElbowResult) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i + 1][j] = rand() % numJewels;
                    grid[i + 1][j - 1] = rand() % numJewels;
                    grid[i + 1][j - 2] = rand() % numJewels;
                    grid[i + 1][j - 3] = rand() % numJewels;
                }
                return score;
            }

            /////////////LEFT-DOWN////////////////////
            // //case 1
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i][j + 2] && grid[i][j + 2] == grid[i + 1][j + 2] && grid[i + 1][j + 2] == grid[i + 2][j + 2])
            {
                isElbow(i, j, i, j + 1, i, j + 2, i + 1, j + 2, i + 2, j + 2);
                if (isElbow) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j + 1] = rand() % numJewels;
                    grid[i][j + 2] = rand() % numJewels;
                    grid[i + 1][j + 2] = rand() % numJewels;
                    grid[i + 2][j + 2] = rand() % numJewels;
                }
                return score;
            }
            //case 2
            else if (grid[i][j] == grid[i][j + 1] && grid[i][j + 1] == grid[i + 1][j + 1] && grid[i + 1][j + 1] == grid[i + 2][j + 1] && grid[i + 2][j + 1] == grid[i + 3][j + 1])
            {
                isElbow(i, j, i, j + 1, i + 1, j + 1, i + 2, j + 1, i + 3, j + 1);
                if (isElbow) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j + 1] = rand() % numJewels;
                    grid[i + 3][j + 1] = rand() % numJewels;
                    grid[i + 1][j + 1] = rand() % numJewels;
                    grid[i + 2][j + 1] = rand() % numJewels;
                }
                return score;
            }
            //case3
            else if (i >= 1 && j >= 3 && grid[i][j] == grid[i - 1][j] && grid[i - 1][j] == grid[i - 1][j - 1] && grid[i - 1][j - 1] == grid[i - 1][j - 2] && grid[i - 1][j - 2] == grid[i - 1][j - 3])
            {
                bool isElbowResult = isElbow(i, j, i - 1, j, i - 1, j - 1, i - 1, j - 2, i - 1, j - 3);
                if (isElbowResult) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i - 1][j] = rand() % numJewels;
                    grid[i - 1][j - 1] = rand() % numJewels;
                    grid[i - 1][j - 2] = rand() % numJewels;
                    grid[i - 1][j - 3] = rand() % numJewels;
                }
                return score;
            }

            ////////////////////FOR RIGHT-UP//////////////////case1
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 2][j] && grid[i + 2][j] == grid[i + 2][j + 1] && grid[i + 2][j + 1] == grid[i + 2][j + 2])
            {
                isElbow(i, j, i + 1, j, i + 2, j, i + 2, j + 1, i + 2, j + 2);
                if (isElbow) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i + 1][j] = rand() % numJewels;
                    grid[i + 2][j] = rand() % numJewels;
                    grid[i + 2][j + 1] = rand() % numJewels;
                    grid[i + 2][j + 2] = rand() % numJewels;
                }
                return score;
            }
            //case2
            else if (grid[i][j] == grid[i + 1][j] && grid[i + 1][j] == grid[i + 1][j + 1] && grid[i + 1][j + 1] == grid[i + 1][j + 2] && grid[i + 1][j + 2] == grid[i + 1][j + 3])
            {
                isElbow(i, j, i + 1, j, i + 1, j + 1, i + 1, j + 2, i + 1, j + 3);
                if (isElbow) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i + 1][j] = rand() % numJewels;
                    grid[i + 1][j + 1] = rand() % numJewels;
                    grid[i + 1][j + 2] = rand() % numJewels;
                    grid[i + 1][j + 3] = rand() % numJewels;
                }
                return score;
            }
            //case3
            else if (i >= 3 && j >= 1 && i < gridSize && j < gridSize &&
                grid[i][j] == grid[i][j - 1] && grid[i][j - 1] == grid[i - 1][j - 1] &&
                grid[i - 1][j - 1] == grid[i - 2][j - 1] && grid[i - 2][j - 1] == grid[i - 3][j - 1])
            {
                bool isElbowResult = isElbow(i, j, i, j - 1, i - 1, j - 1, i - 2, j - 1, i - 3, j - 1);
                if (isElbowResult) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j - 1] = rand() % numJewels;
                    grid[i - 1][j - 1] = rand() % numJewels;
                    grid[i - 2][j - 1] = rand() % numJewels;
                    grid[i - 3][j - 1] = rand() % numJewels;
                }
                return score;
            }
            /////////////FOR RIGHT-DOWN////////////////case1
            else if (i >= 0 && j >= 2 && i + 2 < gridSize && j < gridSize &&
                grid[i][j] == grid[i][j - 1] && grid[i][j - 1] == grid[i][j - 2] &&
                grid[i][j - 2] == grid[i + 1][j - 2] && grid[i + 1][j - 2] == grid[i + 2][j - 2])
            {
                bool isElbowResult = isElbow(i, j, i, j - 1, i, j - 2, i + 1, j - 2, i + 2, j - 2);
                if (isElbowResult) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j - 1] = rand() % numJewels;
                    grid[i][j - 2] = rand() % numJewels;
                    grid[i + 1][j - 2] = rand() % numJewels;
                    grid[i + 2][j - 2] = rand() % numJewels;
                }
                return score;
            }
            //case2
            else if (grid[i][j] == grid[i][j - 1] && grid[i][j - 1] == grid[i - 1][j - 1] && grid[i - 1][j - 1] == grid[i - 2][j - 1] && grid[i - 2][j - 1] == grid[i - 3][j - 1])
            {
                isElbow(i, j, i, j - 1, i - 1, j - 1, i - 2, j - 1, i - 3, j - 1);
                if (isElbow) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i][j - 1] = rand() % numJewels;
                    grid[i - 1][j - 1] = rand() % numJewels;
                    grid[i - 2][j - 1] = rand() % numJewels;
                    grid[i - 3][j - 1] = rand() % numJewels;
                }
                return score;
            }
            //case3
            else if (grid[i][j] == grid[i - 1][j] && grid[i - 1][j] == grid[i - 1][j + 1] && grid[i - 1][j + 1] == grid[i - 1][j + 2] && grid[i - 1][j + 2] == grid[i - 1][j + 3]) {
                bool isElbowResult = isElbow(i, j, i - 1, j, i - 1, j + 1, i - 1, j + 2, i - 1, j + 3);
                if (isElbowResult) {
                    score += 50;
                    grid[i][j] += 10;
                    grid[i - 1][j] = rand() % numJewels;
                    grid[i - 1][j + 1] = rand() % numJewels;
                    grid[i - 1][j + 2] = rand() % numJewels;
                    grid[i - 1][j + 3] = rand() % numJewels;
                }
                return score;
            }
        }
    }
    return score;
}

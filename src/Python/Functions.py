import pygame, math, random
#import cell class
from Cell import *

# turn on pygame
pygame.init()

# window dimensions
WIDTH, HEIGHT = 600, 600
# minesweeper grid dimensions
NX, NY = 10, 10

# side length of each cell
CELL_SIZE = WIDTH / NX

# array of all cells
cells = [[Cell(False,0,0) for i in range(NY)] for j in range(NX)]
# The number of bombs in the game
numBombs = 0

# whether or not the game has ended
game_over = False

# Create the window and fonts
WINDOW = pygame.display.set_mode((WIDTH, HEIGHT))
FONT = pygame.font.Font("assets/DefaultFont.ttf", int(CELL_SIZE/2))
GAMEOVER_FONT = pygame.font.Font("assets/DefaultFont.ttf", int(WIDTH/8))

# Textures for flag and bomb
FLAG = pygame.transform.scale(pygame.image.load("assets/flag.png"), (CELL_SIZE,CELL_SIZE))
BOMB = pygame.transform.scale(pygame.image.load("assets/bomb.png"), (CELL_SIZE,CELL_SIZE))


##############################
#                            #
#      GRID MANAGEMENT       #
#                            #
##############################

# 
def breakBoard():
    # indices of the cell that will be revealed
    bx, by = -1, -1
    
    # iterate through each cell, and update the cell's surrounding count
    # if the number of surrounding bombs is 0, update bx and by
    for x in range(NX):
        for y in range(NY):
            cells[x][y].countSurrounding()
            if cells[x][y].surrounding_bombs == 0:
                bx, by = x, y
                
    # if a cell to reveal was found, reveal it and return true
    if bx>=0 and by>=0:
        cells[bx][by].reveal()
        return True
    # return failure
    return False

def rollForBomb():
    global numBombs
    # If the bomb roll is successful, increment bomb count, and return true
    if random.uniform(0, 1) < 0.25:
        numBombs += 1
        return True
    return False

def createGrid():
    global numBombs, cells
    
    # reset the number of bombs
    numBombs = 0 
    # re-initialise the grid to be empty
    cells = [[Cell(rollForBomb(), j, i) for i in range(NY)] for j in range(NX)]
    
    # break the board, and regenerate the board if it cannot be broken
    if not breakBoard():
        createGrid()

# reveal all cells in the grid. do not reveal bombs that have been correctly flagged
def revealAll():
    for row in cells:
        for cell in row:
            if not cell.revealed:
                if not (cell.isBomb() and cell.flagged):
                    cell.reveal()
                    

##############################
#                            #
#      INPUT FUNCTIONS       #
#                            #
##############################

# left click on the (x,y) coordinates
# left clicking reveals non flagged cells
def leftClick(x, y):
    global game_over
    
    # convert the mouse position to cell indices
    cx, cy = int(x/CELL_SIZE), int(y/CELL_SIZE)
    # return if either index is out of bounds  
    if not (0<=cx<NX and 0<=cy<NY):
        return
    
    # reveal cells that get clicked, don't reveal flagged cells
    cell = cells[cx][cy]
    if not cell.flagged:
        # upon revealing a bomb (reveal returns true), end the game 
        if cell.reveal():
            revealAll()
            game_over = True
      
# right click on the (x, y) coordinates
# right clicking toggles flag on non-revealed cells      
def rightClick(x, y):
    global game_over, numBombs
    
    # convert the mouse position to cell indices
    cx, cy = int(x/CELL_SIZE), int(y/CELL_SIZE)
    # return if either index is out of bounds  
    if not (0<=cx<NX and 0<=cy<NY):
        return
    
    # toggle the flag for cells that get right clicked
    # alter bomb count if toggling flags on a bomb
    # toggle flag returns true if the cell is a bomb
    cell = cells[cx][cy]
    if cell.toggleFlag(): 
        # change bomb count when a bomb is toggled 
        if not cell.flagged:
            numBombs += 1
        else:
            numBombs -= 1
        # win when there are no bombs left
        if numBombs == 0:
            revealAll()
            game_over = True
            
# Gets user input 
def getInput(event):
    global game_over
    
    # when left/right clicking, get mouse position, and call the appropriate function
    if event.type == pygame.MOUSEBUTTONDOWN:
        mousex, mousey = pygame.mouse.get_pos()
        mouse_buttons = pygame.mouse.get_pressed()
        if mouse_buttons[0]:
            leftClick(mousex, mousey)
        elif mouse_buttons[2]:
            rightClick(mousex, mousey)
            
    # If the player presses space, reset the game by creating a new grid
    elif event.type == pygame.KEYDOWN:
        if event.key == pygame.K_SPACE:
            createGrid()
            game_over = False
            

##############################
#                            #
#    RENDERING FUNCTIONS     #
#                            #
##############################


# render all cells in the grid
def renderCells():
    for row in cells:
        for cell in row:
            cell.render()
            
# renders all the grid lines
def renderGrid():
    # render vertical gridlines
    line = pygame.Rect(0, 0, 3, HEIGHT)
    for i in range(NX):
        line.x = CELL_SIZE * i
        pygame.draw.rect(WINDOW, (75,75,75), line)
        
    # render horizontal gridlines
    line = pygame.Rect(0, 0, WIDTH, 3)
    for i in range(NY):
        line.y = CELL_SIZE * i
        pygame.draw.rect(WINDOW, (75,75,75), line)

# renders the entire game  
def render():
    # call rendering helper functions 
    renderCells()
    renderGrid()
    
    # if the game has ended, render the victory or loss text
    if game_over:
        # Create a texture and a rendering rect for the number
        if numBombs == 0:
            txt = GAMEOVER_FONT.render("You Win", True, "green")
            txt_rect = txt.get_rect(center=(WIDTH/2,HEIGHT/2))
            WINDOW.blit(txt, txt_rect)
        else:
            txt = GAMEOVER_FONT.render("You Lose", True, "red")
            txt_rect = txt.get_rect(center=(WIDTH/2,HEIGHT/2))
            WINDOW.blit(txt, txt_rect)
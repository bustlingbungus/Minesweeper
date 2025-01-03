from Cell import *
import random
import math

pygame.init()

# window dimensions
WIDTH, HEIGHT = 600, 600
# minesweeper grid dimensions
NX, NY = 10, 10

# side length of each cell
CELL_SIZE = WIDTH / NX

# create a 2d array of cells
cells = [[Cell(False, []) for i in range(NY)] for j in range(NX)]

game_over = False
numBombs = 0

# create the window and font
WINDOW = pygame.display.set_mode((WIDTH, HEIGHT))
FONT = pygame.font.Font("assets/DefaultFont.ttf", int(CELL_SIZE/2))


##############################
#                            #
#      GRID MANAGEMENT       #
#                            #
##############################

def breakBoard():
    bx, by = -1, -1
    mlen = math.sqrt((NX**2)/4 + (NY**2)/4)
    
    for x in range(NX):
        for y in range(NY):
            if cells[x][y].num_bombs == 0:
                dx, dy = NX/2 - x, NY/2 - y
                dist = math.sqrt(dx**2 + dy**2)
                if dist <= mlen:
                    mlen = dist
                    bx = x
                    by = y
                
    if bx>=0 and by>=0:
        cells[bx][by].reveal()
        return True
    return False
    

# generates the grid containing all cells
def createGrid():
    global numBombs, cells
    
    # reset the number of bombs
    numBombs = 0 
    # re-initialise the grid to be empty
    cells = [[Cell(False, []) for i in range(NY)] for j in range(NX)]
    
    # roll for bombs
    for x in range(NX):
        for y in range(NY):
            bomb_roll = random.uniform(0, 1) < 0.25
            if bomb_roll:
                cells[x][y].makeBomb()
    
    # create cells by iterating through each (x,y) pair
    for x in range(NX):
        for y in range(NY):
            # create an array for the cells surrounding the current cell
            surrounding = []
            for i in range(x-1, x+2):
                if not 0<=i<NX:
                    continue
                for j in range(y-1, y+2):
                    if (not 0<=j<NY) or (j==y and i==x):
                        continue
                    surrounding.append(cells[i][j])
                    cells[x][y].setSurrounding(surrounding)
    
    # regenerate the board if it cannot be broken
    if not breakBoard():
        createGrid()
    

def revealAll():
    # iterate through each cell. If a cell has not been revealed, revealed it
    for row in cells:
        for cell in row:
            if not cell.is_revealed:
                # do not reveal bombs that have been correctly flagged
                if (cell.isBomb() or cell.has_flag):
                    cell.reveal()


##############################
#                            #
#      INPUT FUNCTIONS       #
#                            #
##############################


def leftClick(x, y):
    global game_over
    
    cx, cy = int(x/CELL_SIZE), int(y/CELL_SIZE)
    # return if either index is out of bounds  
    if not (0<=cx<NX and 0<=cy<NY):
        return
    
    # reveal cells that get clicked, don't reveal flagged cells
    cell = cells[cx][cy]
    if not cell.has_flag:
        # upon revealing a bomb (reveal returns true), end the game 
        if cell.reveal():
            revealAll()
            game_over = True
            
def rightClick(x, y):
    cx, cy = int(x/CELL_SIZE), int(y/CELL_SIZE)
    # return if either index is out of bounds  
    if not (0<=cx<NX and 0<=cy<NY):
        return
    
    # toggle the flag for cells that get right clicked
    # alter bomb count if toggling flags on a bomb
    cell = cells[cx][cy]
    if cell.toggleFlag(): 
        if not cell.has_flag():
            numBombs += 1
        else:
            numBombs -= 1
        # win when there are no bombs left
        if numBombs == 0:
            revealAll()
            game_over = True
            
    

def getInput(event):
    if event.type == pygame.MOUSEBUTTONDOWN:
        mousex, mousey = pygame.mouse.get_pos()
        mouse_buttons = pygame.mouse.get_pressed()
        if mouse_buttons[0]:
            leftClick(mousex, mousey)
        elif mouse_buttons[2]:
            rightClick(mousex, mousey)
        
        
##############################
#                            #
#    RENDERING FUNCTIONS     #
#                            #
##############################

def renderCells():
    cell_rect = pygame.Rect(0, 0, CELL_SIZE, CELL_SIZE)
    # iterate through each cell, determine (x,y) coordinates based on cell indices, and render
    for i in range(NX):
        cell_rect.x = CELL_SIZE * i
        for j in range(NY):
            cell_rect.y = CELL_SIZE * j
            cells[i][j].render(WINDOW, FONT, cell_rect)
            
def renderGridlines():
    line = pygame.Rect(0, 0, 3, HEIGHT)
    for i in range(NX):
        line.x = CELL_SIZE * i
        pygame.draw.rect(WINDOW, (75,75,75), line)
        
    line = pygame.Rect(0, 0, WIDTH, 3)
    for i in range(NY):
        line.y = CELL_SIZE * i
        pygame.draw.rect(WINDOW, (75,75,75), line)

def render():
    renderCells()
    renderGridlines()
    # if the game ended, render the win/loss text
    if game_over:
        # Create a texture and a rendering rect for the number
        str = "You Lose"
        col = "red"
        if numBombs == 0:
            str = "You Win"
            col = "green"
        txt = FONT.render(str, True, col)
        txt_rect  = txt.get_rect(center=(WIDTH/2,HEIGHT/2))
        # render the texture
        WINDOW.blit(txt, txt_rect)

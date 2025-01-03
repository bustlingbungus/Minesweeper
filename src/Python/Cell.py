import pygame

def getColor(numSurroundingBombs):
    match (numSurroundingBombs):
        case 1: 
            return "blue"
        case 2:
            return "green"
        case 3: 
            return "red"
        case 4:
            return "purple"
        case 5: 
            return "yellow"
        case 6:
            return "orange"
        case 7: 
            return "black"
        case 8:
            return "black"

class Cell:
    
    def __init__(self, isBomb : bool, x : int, y : int):
        if isBomb:
            self.surrounding_bombs = -1
        else:
            self.surrounding_bombs = 0
        self.revealed = False
        self.flagged = False
        self.x, self.y = x, y
        
    def isBomb(self) -> bool:
        return self.surrounding_bombs == -1
    
    def toggleFlag(self) -> bool:
        self.flagged = not self.flagged
        return self.isBomb()
    
    def surrounding(self):
        from Functions import cells, NX, NY
        surrounding = []
        for i in range(self.x-1, self.x+2):
            if not 0<=i<NX:
                continue
            for j in range(self.y-1, self.y+2):
                if (not 0<=j<NY) or (j==self.y and i==self.x):
                    continue
                surrounding.append(cells[i][j])
        return surrounding

    def countSurrounding(self):
        if not self.isBomb():
            self.surrounding_bombs = 0
            for cell in self.surrounding():
                if cell.isBomb():
                    self.surrounding_bombs += 1
        
    def reveal(self) -> bool:
        # make the cell revealed
        self.revealed = True
        # If there are not surrounding bombs, reveal all surrounding
        if self.surrounding_bombs == 0:
            for cell in self.surrounding():
                if not cell.revealed:
                    cell.reveal()
                    
        # return whether the cell is a bomb
        return self.isBomb()
    
    def render(self):
        from Functions import WINDOW, FONT, CELL_SIZE, FLAG, BOMB
        rect = pygame.Rect(self.x*CELL_SIZE, self.y*CELL_SIZE, CELL_SIZE, CELL_SIZE)
        
        if self.revealed:
            pygame.draw.rect(WINDOW, (170,170,170), rect)
            if self.isBomb():
                WINDOW.blit(BOMB, rect)
            elif self.surrounding_bombs > 0:
                num = FONT.render(str(self.surrounding_bombs), True, getColor(self.surrounding_bombs))
                num_rect = num.get_rect(center=(rect.x+rect.w/2,rect.y+rect.h/2))
                WINDOW.blit(num, num_rect)
        else:
            pygame.draw.rect(WINDOW, (90,90,90), rect)
            if self.flagged:
                WINDOW.blit(FLAG, rect)
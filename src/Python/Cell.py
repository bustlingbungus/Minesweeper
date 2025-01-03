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
    
    def __init__(self, isBomb : bool, surrounding):
        self.surrounding = surrounding
        
        self.num_bombs = 0
        self.is_revealed = False
        self.has_flag = False
            
    def setSurrounding(self, surrounding):
        self.surrounding = surrounding
        self.countSurrounding()
            
    def makeBomb(self):
        self.num_bombs = -1
            
    def isBomb(self):
        return self.num_bombs == -1
    
    def toggleFlag(self):
        self.has_flag = not self.has_flag
        return self.isBomb()
    
    def countSurrounding(self):
        if not self.isBomb():
            # reset counter
            self.num_bombs = 0
            # for each surrounding cell, increment counter for each bomb
            for cell in self.surrounding:
                if cell.isBomb():
                    self.num_bombs += 1
                    
    def reveal(self):
        if self.is_revealed:
            print("Cell already revealed")
        else:
            print("Revealing cell")
        # make the cell revealed
        self.is_revealed = False
        # If there are not surrounding bombs, reveal all surrounding
        if self.num_bombs == 0:
            for cell in self.surrounding:
                if not cell.is_revealed:
                    cell.reveal
                    
        # return whether the cell is a bomb
        return self.isBomb()
      
            
    def render(self, screen : pygame.display.set_mode, font : pygame.font.Font, square : pygame.Rect):
        # render cell information if revealed
        if self.is_revealed:
            # render background first
            pygame.draw.rect(screen, (170,170,170), square)
            # render a bomb if cell is a bomb, render the number of bombs otherwise
            if self.isBomb():
                print("",end="")
            elif self.num_bombs > 0:
                # Create a texture and a rendering rect for the number
                num = font.render(self.num_bombs, True, getColor(self.num_bombs))
                num_rect  = num.get_rect(center=(square.x-square.w/2,square.y-square.h/2))
                # render the texture
                screen.blit(num, num_rect)
                
        # if hidden, just render background, and any flag
        else:
            pygame.draw.rect(screen, (90,90,90), square)
        
    
import pygame, sys
from Cell import *
from Functions import *

createGrid()

quit_app = False

while not quit_app:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            quit_app = True
        else:
            getInput(event)
            
    WINDOW.fill("black")
    
    render()
    
    pygame.display.update()
    
pygame.quit()
sys.exit()
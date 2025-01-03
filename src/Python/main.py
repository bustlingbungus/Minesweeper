import pygame, sys
from Cell import *
from Functions import *

createGrid()

# Window dimensions. Set up the window, and a clock
CLOCK = pygame.time.Clock()

quit_app = False

# Main simulation loop
while (not quit_app):
    
    # Handle input
    for event in pygame.event.get():
        # Exit when you press the X
        if event.type == pygame.QUIT:
            quit_app = True
        else:
            getInput(event)

    # Fill the window with a solid colour
    WINDOW.fill("white")
    
    render()

    # Update the window
    pygame.display.update()
    CLOCK.tick(300)

# Close the application when the main loop exits
pygame.quit()
sys.exit()
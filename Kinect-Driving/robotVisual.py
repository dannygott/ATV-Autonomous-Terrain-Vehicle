
import pygame

pygame.init()

# Displays are measured in pixels.
DisplayWidth = 1000
DisplayHeight = 1000
center = [0, 0]
center[0] = int(DisplayWidth/2)
center[1] = int(DisplayHeight/2)
jaun = pygame.Surface((500, 300))
# Colors are mixes of red, green and blue. 0 is none.  255 is full intensity.
black = (0, 0, 0)
white = (255, 255, 255)
degrees = 0
MyDisplay = pygame.display.set_mode((DisplayWidth, DisplayHeight))

pygame.display.set_caption('ATV Visual')

# Load in a picture.
# Figure out the size of the image.


running = True
degrees += degrees
while running:
    pygame.event.pump()
    mouse_buttons = pygame.mouse.get_pressed()
    if mouse_buttons[0]:
        running = False

    MyDisplay.fill(black)
    jaun.fill((55, 155, 255))
    pygame.draw.circle(MyDisplay, white, center, center[0])

    pygame.draw.rect(jaun, (0, 0, 100), ((
        center[0]-15, center[1]-25), (50, 50)))
    pygame.transform.rotate(jaun, 20)
    MyDisplay.blit(jaun, (0, 0))

    # Flip the double buffer.
    pygame.display.flip()

pygame.quit()

import pygame
import numpy as np
import random
import math

# The task of this project is to solve Symetric Travelling Salesman Problem ( TSP ) using
# nonsystematic state space search algorithm ( simulated annealing, SA in this case ) and
# create a visualization of the process

def draw_point_and_line(screen, prev_point, point, color=(0, 0, 0)):
    pygame.draw.circle(screen, color, point, 5)
    pygame.draw.line(screen, color, prev_point, point, 2)
    pygame.display.update()


# This function is used to print a path on the screen
def print_current_path(screen, cities, font, path, distance, time_to_wait, info, temp):
    screen.fill((255, 255, 255))
    pygame.display.update()
    for i, city in enumerate(cities):
        color = (0, 0, 0)
        pygame.draw.circle(screen, color, city, 5)
        city_id = font.render(str(i), True, color)
        screen.blit(city_id, city)
        pygame.display.update()
        pygame.time.wait(100)

    for i in range(len(path) - 1):
        pygame.draw.line(screen, (0, 255, 0), cities[path[i]], cities[path[i + 1]], 2)
        pygame.display.update()
        pygame.time.wait(300)

    path_text = font.render(info + str(path), True, (0, 0, 0))
    distance_text = font.render("Distance: " + str(distance), True, (0, 0, 0))
    temperature_text = font.render("Current temperature is: " + str(temp), True, (0, 0, 0))
    screen.blit(distance_text, (10, 10))
    screen.blit(path_text, (10, 50))
    screen.blit(temperature_text, (10, 90))
    pygame.display.update()
    pygame.time.wait(time_to_wait)


# This function calculates the distance of the whole path ( how long it takes to visit all the cities )
def total_distance(path, distance_matrix):
    return sum(distance_matrix[path[i - 1]][path[i]] for i in range(len(path)))


# This function implements SA algorithm and show each step of generating a path
def start_sa_and_show_steps(screen, cities, font, distance_matrix):
    path = list(range(len(cities)))
    random.shuffle(path)
    path.append(path[0])
    best_path = list(path)
    best_distance = total_distance(best_path, distance_matrix)
    screen.fill((255, 255, 255))
    pygame.display.update()
    temperature_curr = 10
    temperature_min = 1
    low_temp_on = 0.8
    print_current_path(screen, cities, font, best_path, best_distance, 1000, "Start path ", temperature_curr)
    while temperature_curr > temperature_min:
        new_path = list(path)
        a, b = random.sample(range(1, len(cities)), 2)
        new_path[a], new_path[b] = new_path[b], new_path[a]

        prev_distance = total_distance(path, distance_matrix)
        new_distance = total_distance(new_path, distance_matrix)
        print_current_path(screen, cities, font, path, prev_distance, 1000, "Previous path ", temperature_curr)
        draw_point_and_line(screen, cities[path[a]], cities[path[b]], (255, 0, 0))
        pygame.time.wait(1000)
        print_current_path(screen, cities, font, new_path, new_distance, 1000, "New path ", temperature_curr)
        pygame.display.update()
        pygame.time.wait(1000)
        if new_distance < prev_distance:
            path = new_path
        else:
            probability = math.exp((prev_distance - new_distance) / temperature_curr)
            if random.random() < probability:
                path = new_path

        if new_distance < best_distance:
            best_distance = new_distance
            best_path = list(path)
        temperature_curr *= low_temp_on

    print_current_path(screen, cities, font, best_path, best_distance, 10000, "Final path ", temperature_curr)


# This function runs the process and reads data from the file
if __name__ == "__main__":
    pygame.init()
    main_screen = pygame.display.set_mode((800, 600))
    matrix_with_city_distances = []

    with open('matrix.txt', 'r') as file:
        for line in file:
            row = [int(num) for num in line.split()]
            matrix_with_city_distances.append(row)

    width, height = pygame.display.get_surface().get_size()
    all_cities = np.random.randint(100, min(width, height) - 100, size=(len(matrix_with_city_distances), 2))

    font_for_screen = pygame.font.Font(None, 30)

    start_sa_and_show_steps(main_screen, all_cities, font_for_screen, matrix_with_city_distances)

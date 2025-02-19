#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000

// Directions for neighbors (right, down, left, up)
int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// Grid dimensions
int rows, cols;
char grid[MAX_ROWS][MAX_COLS];
bool visited[MAX_ROWS][MAX_COLS];
bool edges[MAX_ROWS * MAX_COLS * 4]; // To track the uniqueness of edges

// Utility function to check bounds
bool is_within_bounds(int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Utility to calculate the unique edge identifier
int edge_id(int x, int y, int direction) {
    return (x * cols + y) * 4 + direction;
}

// Flood-fill to calculate area and sides
void flood_fill(int x, int y, char plant, int *area, int *sides) {
    visited[x][y] = true;
    (*area)++;

    for (int d = 0; d < 4; d++) {
        int nx = x + directions[d][0];
        int ny = y + directions[d][1];

        if (!is_within_bounds(nx, ny) || grid[nx][ny] != plant) {
            // Add edge if it's unique
            int id = edge_id(x, y, d);
            if (!edges[id]) {
                edges[id] = true;
                (*sides)++;
            }
        } else if (!visited[nx][ny]) {
            flood_fill(nx, ny, plant, area, sides);
        }
    }
}

// Main function for Part 2
int main() {
    FILE *file = fopen("inputdata.txt", "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    rows = 0;
    while (fgets(grid[rows], MAX_COLS, file)) {
        grid[rows][strcspn(grid[rows], "\n")] = '\0';
        rows++;
    }
    fclose(file);

    cols = strlen(grid[0]);
    memset(visited, 0, sizeof(visited));
    memset(edges, 0, sizeof(edges));

    int total_cost = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                int area = 0, sides = 0;
                flood_fill(i, j, grid[i][j], &area, &sides);
                total_cost += area * sides;
            }
        }
    }

    printf("Part 2 Total Cost: %d\n", total_cost);
    return 0;
}

import matplotlib.pyplot as plt
import numpy as np

def read_waypoints():
    waypoints = {}
    with open('waypoint.txt', 'r') as file:
        lines = file.readlines()
        for line in lines:
            parts = line.split()
            waypoint = int(parts[0][2:])  # Ottieni il numero del waypoint
            x, y, _ = map(float, parts[1][1:-1].split(','))  # Ottieni le coordinate x e y
            waypoints[waypoint] = (x, y)
    return waypoints

def read_connections():
    connections = []
    with open('connections.txt', 'r') as file:
        lines = file.readlines()
        for line in lines:
            if line.startswith("Waypoint"):
                parts = line.split(":")
                waypoint = int(parts[0].split(" ")[1])
                connected_waypoints = [int(wp) for wp in parts[1].split()]
                connections.append((waypoint, connected_waypoints))
    return connections

def plot_connections(waypoints, connections):
    plt.figure(figsize=(8, 8))
    plt.title("Waypoint Connections")
    plt.xlabel("X")
    plt.ylabel("Y")

    # Imposta i limiti del grafico tra -3 e +3
    plt.xlim(-3, 3)
    plt.ylim(-3, 3)

    # Disegna i quadrati neri nei quattro angoli
    corners = np.array([[-3, -3], [-3, 2], [2, 2], [2, -3]])
    for corner in corners:
        plt.fill([corner[0], corner[0]+1, corner[0]+1, corner[0]], [corner[1], corner[1], corner[1]+1, corner[1]+1], color='grey')

    # Dizionario per assegnare un colore univoco a ciascun waypoint
    waypoint_colors = {}

    for waypoint, connected_waypoints in connections:
        x, y = waypoints[waypoint]
        
        # Assegna un colore univoco al waypoint se non è già stato assegnato
        if waypoint not in waypoint_colors:
            waypoint_colors[waypoint] = plt.cm.get_cmap('tab20')(len(waypoint_colors) % 20)

        plt.scatter(x, y, color=waypoint_colors[waypoint], label=f'Waypoint {waypoint}')
        plt.annotate(str(waypoint), (x, y), textcoords="offset points", xytext=(0,10), ha='center')
        
        for connected_waypoint in connected_waypoints:
            cx, cy = waypoints[connected_waypoint]
            plt.plot([x, cx], [y, cy], color='black')

    # plt.legend()
    plt.show()

waypoints = read_waypoints()
connections = read_connections()
plot_connections(waypoints, connections)
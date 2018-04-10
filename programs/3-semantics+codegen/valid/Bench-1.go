package main

// Stations
var stations []string

// Graph struct
type Graph struct {
    nodes int
    al [][]int
    weight [][]float64
    deg []int
}

// Build a graph
var graph Graph

/**
 * Add node to a graph
 */
func addNode() int {
    var neighbors []int
    var weights []float64
    graph.al = append(graph.al, neighbors);
    graph.weight = append(graph.weight, weights);
    graph.deg = append(graph.deg, 0)
    graph.nodes++
    return graph.nodes-1;
}

/**
 * Add edge to a graph
 */
func addEdge(from, to int, w float64) {
    graph.al[from] = append(graph.al[from], to)
    graph.weight[from] = append(graph.weight[from], w)
    graph.deg[from]++
}

/**
 * Connect two nodes
 */
func connect(from, to int, w float64) {
    addEdge(from, to, w)
    addEdge(to, from, w)
}

/**
 * Print path
 */
func printPath(path []int) {
    for i:=0; path[i] >= 0; i++ {
        if i != 0 {
            print(" -> ")
        }
        print(stations[path[i]])
    }
    println()
}

/**
 * Run Dijkstra
 */
func dijkstra(from ,to int) []int {
    // Keep track of visited nodes
    var visited []int
    var UNVISITED int = 0
    var VISITING int = 1
    var VISITED int = 2
    for i:=0; i < graph.nodes; i++ {
        visited = append(visited, UNVISITED)
    }

    // Keep track of cost
    var cost[] float64
    var MAX float64 = 99999.;
    for i:=0; i < graph.nodes; i++ {
        cost = append(cost, MAX)
    }

    // Keep track of parents
    var parent[] int
    for i:=0; i < graph.nodes; i++ {
        parent = append(parent, -1)
    }

    // Queue of node
    var queue []int
    var queueLen = 0
    
    // Allocate space in queue
    for i:=0; i < graph.nodes; i++ {
        queue = append(queue, 0)
    }

    // Queue start node
    queue[0] = from
    visited[from] = VISITING
    queueLen = 1

    // Queue from node
    for queueLen > 0 {
        
        // Search for min weight
        var min int = 0;
        for i := 1; i < queueLen; i++ {
            if cost[queue[i]] < cost[queue[min]] {
                min = i
            }
        }
        var top = queue[min]

        // Remove top from queue
        queue[min], queue[queueLen-1] = queue[queueLen-1], queue[min]
        queueLen--

        // Mark it as visited
        visited[top] = VISITED

        // Update and queue neighbors
        for i := 0; i < graph.deg[top]; i++ {
            var neighbor int = graph.al[top][i]
            var newCost float64 = cost[top] + graph.weight[top][i]
            if visited[neighbor] == UNVISITED {
                cost[neighbor] = newCost
                visited[neighbor] = VISITING
                parent[neighbor] = top
                queue[queueLen] = neighbor
                queueLen++
            } else if visited[neighbor] == VISITING {
                if cost[neighbor] > newCost {
                    cost[neighbor] = newCost
                    parent[neighbor] = top
                }
            }
        }
    }

    // Create reverse path
    var pathRev []int

    // If not reachable
    if parent[to] < 0 {
        pathRev = append(pathRev, -1)
        return pathRev
    }

    // If reachable
    var count int = 0
    pathRev = append(pathRev, to)
    count++
    var tmpParent = parent[to]
    for tmpParent >= 0 {
        pathRev = append(pathRev, tmpParent)
        tmpParent = parent[tmpParent]
        count++
    }
    
    // Reverse path
    var path []int
    for i:=count; i > 0; i-- {
        path = append(path, pathRev[i-1])
    }
    path = append(path, -1)
    return path
}

func main() {

    // Populate stations
    stations = append(stations, "Acadie")
    stations = append(stations, "Angrignon")
    stations = append(stations, "Assomption")
    stations = append(stations, "Atwater")
    stations = append(stations, "Beaubien")
    stations = append(stations, "Beaudry")
    stations = append(stations, "Berri-UQAM")
    stations = append(stations, "Bonaventure")
    stations = append(stations, "Cadillac")
    stations = append(stations, "Cartier")
    stations = append(stations, "Champ-de-Mars")
    stations = append(stations, "Charlevoix")
    stations = append(stations, "Côte-des-Neiges")
    stations = append(stations, "Côte-Sainte-Catherine")
    stations = append(stations, "Côte-Vertu")
    stations = append(stations, "Crémazie")
    stations = append(stations, "D'Iberville")
    stations = append(stations, "De Castelnau")
    stations = append(stations, "De L'Église")
    stations = append(stations, "De La Concorde")
    stations = append(stations, "De La Savane")
    stations = append(stations, "Du Collège")
    stations = append(stations, "Édouard-Montpetit")
    stations = append(stations, "Fabre")
    stations = append(stations, "Frontenac")
    stations = append(stations, "Georges-Vanier")
    stations = append(stations, "Guy-Concordia")
    stations = append(stations, "Henri-Bourassa")
    stations = append(stations, "Honoré-Beaugrand")
    stations = append(stations, "Jarry")
    stations = append(stations, "Jean-Drapeau")
    stations = append(stations, "Jean-Talon")
    stations = append(stations, "Jolicoeur")
    stations = append(stations, "Joliette")
    stations = append(stations, "Langelier")
    stations = append(stations, "LaSalle")
    stations = append(stations, "Laurier")
    stations = append(stations, "Lionel-Groulx")
    stations = append(stations, "Longueuil–Université-de-Sherbrooke")
    stations = append(stations, "Lucien-L'Allier")
    stations = append(stations, "McGill")
    stations = append(stations, "Monk")
    stations = append(stations, "Mont-Royal")
    stations = append(stations, "Montmorency")
    stations = append(stations, "Namur")
    stations = append(stations, "Outremont")
    stations = append(stations, "Papineau")
    stations = append(stations, "Parc")
    stations = append(stations, "Peel")
    stations = append(stations, "Pie-IX")
    stations = append(stations, "Place-d'Armes")
    stations = append(stations, "Place-des-Arts")
    stations = append(stations, "Place-Saint-Henri")
    stations = append(stations, "Plamondon")
    stations = append(stations, "Préfontaine")
    stations = append(stations, "Radisson")
    stations = append(stations, "Rosemont")
    stations = append(stations, "Saint-Laurent")
    stations = append(stations, "Saint-Michel")
    stations = append(stations, "Sauvé")
    stations = append(stations, "Sherbrooke")
    stations = append(stations, "Snowdon")
    stations = append(stations, "Square-Victoria-OACI")
    stations = append(stations, "Université-de-Montréal")
    stations = append(stations, "Vendôme")
    stations = append(stations, "Verdun")
    stations = append(stations, "Viau")
    stations = append(stations, "Villa-Maria")

    // Create stations
    var n0  int = addNode();
    var n1  int = addNode();
    var n2  int = addNode();
    var n3  int = addNode();
    var n4  int = addNode();
    var n5  int = addNode();
    var n6  int = addNode();
    var n7  int = addNode();
    var n8  int = addNode();
    var n9  int = addNode();
    var n10 int = addNode();
    var n11 int = addNode();
    var n12 int = addNode();
    var n13 int = addNode();
    var n14 int = addNode();
    var n15 int = addNode();
    var n16 int = addNode();
    var n17 int = addNode();
    var n18 int = addNode();
    var n19 int = addNode();
    var n20 int = addNode();
    var n21 int = addNode();
    var n22 int = addNode();
    var n23 int = addNode();
    var n24 int = addNode();
    var n25 int = addNode();
    var n26 int = addNode();
    var n27 int = addNode();
    var n28 int = addNode();
    var n29 int = addNode();
    var n30 int = addNode();
    var n31 int = addNode();
    var n32 int = addNode();
    var n33 int = addNode();
    var n34 int = addNode();
    var n35 int = addNode();
    var n36 int = addNode();
    var n37 int = addNode();
    var n38 int = addNode();
    var n39 int = addNode();
    var n40 int = addNode();
    var n41 int = addNode();
    var n42 int = addNode();
    var n43 int = addNode();
    var n44 int = addNode();
    var n45 int = addNode();
    var n46 int = addNode();
    var n47 int = addNode();
    var n48 int = addNode();
    var n49 int = addNode();
    var n50 int = addNode();
    var n51 int = addNode();
    var n52 int = addNode();
    var n53 int = addNode();
    var n54 int = addNode();
    var n55 int = addNode();
    var n56 int = addNode();
    var n57 int = addNode();
    var n58 int = addNode();
    var n59 int = addNode();
    var n60 int = addNode();
    var n61 int = addNode();
    var n62 int = addNode();
    var n63 int = addNode();
    var n64 int = addNode();
    var n65 int = addNode();
    var n66 int = addNode();
    var n67 int = addNode();

    // Connect stations
    connect(0,47,727.60)
    connect(1,41,844.29)
    connect(2,8,781.69)
    connect(3,26,681.54)
    connect(4,31,712.10)
    connect(5,46,495.00)
    connect(6,5,378.76)
    connect(6,30,2362.10)
    connect(6,60,579.10)
    connect(7,62,392.60)
    connect(8,34,518.51)
    connect(9,19,2073.60)
    connect(10,6,720.50)
    connect(11,37,1077.31)
    connect(15,59,1279.60)
    connect(12,63,764.60)
    connect(13,61,693.00)
    connect(14,21,777.24)
    connect(16,58,607.60)
    connect(17,31,471.60)
    connect(18,35,812.30)
    connect(19,43,847.60)
    connect(20,44,786.70)
    connect(21,20,1281.69)
    connect(23,16,644.50)
    connect(24,54,1003.95)
    connect(25,39,530.60)
    connect(26,48,593.14)
    connect(27,9,1101.60)
    connect(29,15,825.60)
    connect(30,38,1572.10)
    connect(31,23,839.60)
    connect(31,29,977.10)
    connect(32,65,761.39)
    connect(33,49,766.88)
    connect(34,55,621.79)
    connect(35,11,707.25)
    connect(36,56,746.10)
    connect(37,3,1387.74)
    connect(37,25,758.60)
    connect(39,7,381.60)
    connect(40,51,345.69)
    connect(41,32,1062.85)
    connect(42,36,499.60)
    connect(44,53,988.47)
    connect(45,0,728.60)
    connect(46,24,1157.57)
    connect(47,17,490.60)
    connect(48,40,296.52)
    connect(49,66,621.85)
    connect(50,10,370.60)
    connect(51,57,354.38)
    connect(52,37,579.60)
    connect(53,13,451.10)
    connect(54,33,383.43)
    connect(55,28,716.99)
    connect(56,4,541.10)
    connect(57,6,336.80)
    connect(59,27,771.60)
    connect(60,42,932.10)
    connect(61,12,959.60)
    connect(61,67,884.41)
    connect(62,50,356.60)
    connect(63,22,667.60)
    connect(64,52,1450.88)
    connect(65,18,563.86)
    connect(66,2,895.87)
    connect(67,64,1407.32)
    connect(22,45,1090.60)

    // Print path
    printPath(dijkstra(n61, n58))
    printPath(dijkstra(n58, n61))
}

func main2() {
    stations = append(stations, "a")
    stations = append(stations, "b")
    stations = append(stations, "c")
    stations = append(stations, "d")
    stations = append(stations, "e")
    stations = append(stations, "f")
    stations = append(stations, "g")

    var a = addNode()
    var b = addNode()
    var c = addNode()
    var d = addNode()
    var e = addNode()
    var f = addNode()
    var g = addNode()

    connect(a, b, 4.)
    connect(a, c, 3.)
    connect(b, e, 12.)
    connect(b, f, 5.)
    connect(c, d, 7.)
    connect(c, e, 10.)
    connect(d, e, 2.)
    connect(e, g, 5.)
    connect(f, g, 16.)
    
    printPath(dijkstra(a, e))
    printPath(dijkstra(e, a))
}

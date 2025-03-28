#pragma once
template <unsigned int dim>
graph<dim>::graph(const std::string& file_path) : m{} {
    std::ifstream file(file_path);
    if (!file) {
        throw std::runtime_error("Error: Unable to open file " + file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "c") {
            continue;
        } else if (type == "p") {
            std::string format;
            unsigned int nodes, edges;
            iss >> format >> nodes >> edges;
            if (format != "edge" || nodes != dim) {
                throw std::runtime_error("Error: Dimension mismatch in file");
            }
        } else if (type == "e") {
            unsigned int u, v;
            if (iss >> u >> v) {
                --u;
                --v;
                m[u][v] = true;
                m[v][u] = true;
            }
        }
    }
}

template <unsigned int dim>
graph<dim>::graph(const double density) : m{} {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(density); // Probability of an edge

    for (unsigned int i = 0; i < dim; ++i) {
        for (unsigned int j = i + 1; j < dim; ++j) { // Fill only upper triangle
            bool edge = dist(gen);
            m[i][j] = edge;
            m[j][i] = edge; // Symmetric for undirected graph
        }
        m[i][i] = false; // No self-loops
    }
}

template<unsigned int dim>
bool graph<dim>::operator()(unsigned int i, unsigned int j) const {
    return m[i][j];
}

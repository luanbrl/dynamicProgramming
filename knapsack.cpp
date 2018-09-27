#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Knapsack {
  int num_items_;
  int capacity_;
  int *weight;
  int *value;
};

struct Cell{
  int objective_;
  bool x;
};

inline void getData(struct Knapsack &knapsack, const string &filePath) {
  ifstream file(filePath);

  file >> knapsack.num_items_;
  knapsack.weight = new int[knapsack.num_items_];
  knapsack.value = new int[knapsack.num_items_];
  file >> knapsack.capacity_;

  int i, j, k=0;
  while (file >> i >> j) {
    knapsack.weight[k] = i;
    knapsack.value[k] = j;
    k++;
  }
}

int Knapsack(const Knapsack &knapsack) {
  vector<vector<Cell>> table_vector(knapsack.capacity_+1, vector<Cell>(knapsack.num_items_+1));
  vector<Cell> *K = table_vector.data();

  for (int i=knapsack.num_items_; i>=0; i--) {
    for (int w=0; w<=knapsack.capacity_ ; w++) {
      if ((i==knapsack.num_items_) || w==0) {
        K[w][i].objective_ = 0;
      } else if (knapsack.weight[i] <= w) {
        if(K[w][i+1].objective_ > (knapsack.value[i]+K[w-knapsack.weight[i]][i+1].objective_)) {
          K[w][i].objective_ = K[w][i+1].objective_;
          K[w][i].x = false;
        } else {
          K[w][i].objective_ = knapsack.value[i] + K[w - knapsack.weight[i]][i+1].objective_;
          K[w][i].x = true;
        }
      } else {
        K[w][i].objective_ = K[w][i+1].objective_;
        K[w][i].x = false;
      }
    }
  }

  int bestValue = K[knapsack.capacity_][0].objective_;
  int line = knapsack.capacity_;
  int column = 0;

  while(bestValue>0) {
    if (!K[line][column].x) {
      column++;
    } else {
      cout << "Produto " << column + 1 << " usado" << endl;
      line = line - knapsack.weight[column];
      bestValue -= knapsack.value[column];
      column++;
    }
  }
  return K[knapsack.capacity_][0].objective_;
}

int main() {

  string fileName;
  cout << "Digite o nome do arquivo: ";
  cin >> fileName;
  string filePath = "instancias/" + fileName;
  ifstream input(filePath);

  if (input.is_open()) {
    struct Knapsack knapsack;
    getData(knapsack, filePath);

    clock_t begin = clock();
    cout << "Lucro total: " << Knapsack(knapsack) << endl;
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Tempo gasto: " << time_spent << " segundos" << endl;

  } else cout << "Erro ao abrir o arquivo." << endl;

  return 0;

}

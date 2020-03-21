#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <fstream>
#include <sstream>
#include <tuple>

// This method returns all primes up to a given point (using sieve of Eratosthenes algorithm)
std::vector<int> sieve_of_eratosthenes(uint32_t num)
{
    std::vector<int> result {1};

    for (int i=2; i<=num; i++) {
        bool prime = true;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                prime = false;
                break;
            }
        }
        if (prime) result.push_back(i);
    }
    return result;
}

// This method takes two iterators and creates a new vector from the summation of the iterator values
// it_1 must belong to a larger vector than it_2
std::vector<int> sum_vectors(std::vector<int>::iterator it_1,
                             std::vector<int>::iterator it_1_end,
                             std::vector<int>::iterator it_2,
                             std::vector<int>::iterator it_2_end)
{
    std::vector<int> summed_vector(it_1, it_1_end);

    std::transform(it_2, it_2_end, summed_vector.begin()+1, summed_vector.begin()+1,
                   [](int &a, int &b) { return a + b; });

    return summed_vector;
}

int solve_coin_change(std::vector<int> &coins, const int &target, const int &min_comb, const int &max_comb) {

    // This 3D vector contains coins by 0-target by a vector that contains how many combinations there are for a certain
    // length where the indexes are the lengths and the value is how many combinations, an example is
    // [0,2,1] which translates to 0 combinations of length 0, 2 combination of length 1 and 1 combination of length 2
    std::vector<std::vector<std::vector<int>>> table(coins.size() + 1, std::vector<std::vector<int>>(target + 1));

    // Set the first row to a [1] followed by [0,0], [0,0,0], [0,0,0,0]....
    table[0][0].push_back(1);
    for (int i = 1; i <= target; i++) {
        for (int j = 0; j <= i; j++)
            table[0][i].push_back(0);
    }

    // Run the dynamic solver
    for (int row=1; row<=coins.size(); row++) {
        for (int col=0; col<=target; col++) {

            // Take the value from above in the table
            std::vector<int> &y = table[row-1][col];

            // Check if we can use a sub problem in the same row
            if (col - coins[row-1] >= 0)
            {
                // Get the value at column minus current coin
                std::vector<int> &x = table[row][col-coins[row-1]];

                // Add the vectors to get the new sub problem vector
                table[row][col] = sum_vectors(y.begin(), y.end(), x.begin(), x.end());
            }
            else
            {
                // Else we just use the above vector only
                for (int len : y)
                    table[row][col].push_back(len);
            }

        }
    }

    // Get the combinations from length = min to length = max
    int num_combinations = 0;
    for (int i=min_comb; i<=max_comb; i++)
        num_combinations += table[coins.size()][target][i];

    return num_combinations;

}

// This method is used to parse the input file and returns a list of tuples of the data in the format (target, min, max)
std::list<std::tuple<int,int,int>> get_file_input(std::string &filename)
{
    // List of tuples
    std::list<std::tuple<int,int,int>> data;

    std::ifstream in_file;
    in_file.open(filename);

    // A line
    std::string str;

    while (std::getline(in_file, str))
    {
        // Split the string by spaces
        std::string buf; // buffer string
        std::istringstream iss(str);
        int i = 0; // count to see which value we are at on the line
        int amount = 0; int min = 0; int max = 0;
        while (iss >> buf) {
            if (i==0)
                amount = std::stoi(buf);
            else if (i==1)
                min = std::stoi(buf);
            else
                max = std::stoi(buf);
            i++;
        }

        if (max == 0)
            max = amount;

        data.emplace_back(amount, min, max);
    }
    in_file.close();
    return data;
}


int main() {

    std::string filename = "input.txt";

    // For each line of data in the file
    for (auto data : get_file_input(filename))
    {
        // data values
        int amount = std::get<0>(data);
        int min_c = std::get<1>(data);
        int max_c = std::get<2>(data);

        // Get the primes numbers from 1 to amount
        std::vector<int> primes = sieve_of_eratosthenes(amount);

        // Track algorithm run time
        auto startClock = std::chrono::high_resolution_clock::now();

        int solution = solve_coin_change(primes, amount, min_c, max_c);

        auto endClock = std::chrono::high_resolution_clock::now();
        double microseconds = std::chrono::duration_cast<std::chrono::microseconds> (endClock - startClock).count();

        // Print the solution
        std::cout << "Solution for $" << amount << " with combination range ("<<min_c<<","<<max_c<<"): "
        <<solution << "\nRun time: " << double(microseconds) / 1000000<< " seconds.\n"  << std::endl;
    }


    return 0;
}

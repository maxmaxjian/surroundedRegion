#include <iostream>
#include <vector>
#include <utility>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <functional>

using pos = std::pair<size_t,size_t>;

namespace std {
    template<>
    struct hash<pos> {
	size_t operator()(const pos & p) const {
	    return std::hash<size_t>()(p.first)^std::hash<size_t>()(p.second);
	}
    };
}

namespace std {
    std::ostream & operator<<(std::ostream & os, const pos & p) {
        os << "(" << p.first << "," << p.second << ")";
        return os;
    }
}

class solution {
public:
    void solve(std::vector<std::string> & board) {
	std::cout << "Before capture:\n";
	std::copy(board.begin(), board.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	
	auto regions = findRegions(board);
	std::for_each(regions.begin(), regions.end(),
		      [&](const std::unordered_set<pos> & st){
			  std::for_each(st.begin(), st.end(), [&](const pos & p){
				  board[p.first][p.second] = 'X';
			      });
		      });
	
	std::cout << "After capture:\n";
	std::copy(board.begin(), board.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    }

private:
    std::vector<std::unordered_set<pos>> findRegions(const std::vector<std::string> & board) {
	std::unordered_set<pos> holes;
	for (size_t i = 0; i < board.size(); i++)
	    for (size_t j = 0; j < board[i].size(); j++)
		if (board[i][j] == 'O')
		    holes.insert(std::make_pair(i,j));

	std::vector<std::unordered_set<pos>> regions;
	while (!holes.empty()) {
	    auto it = holes.begin();
	    std::unordered_set<pos> region = grow(std::unordered_set<pos>{*it}, holes);
	    for (auto ite = region.begin(); ite != region.end(); ite++)
		holes.erase(*ite);
            std::copy(region.begin(), region.end(), std::ostream_iterator<pos>(std::cout, " "));
            std::cout << std::endl;
	    if (isRegionSurrounded(board, region))
		regions.push_back(region);
	}
	
	return regions;
    }

    bool isRegionSurrounded(const std::vector<std::string> & board, const std::unordered_set<pos> & regions) {
	return std::any_of(regions.begin(), regions.end(),
			   [&](const pos & p){
			       return !(p.first == 0 || p.first == board.size()-1 || p.second == 0 || p.second == board[0].size()-1);
			   });
    }

    std::unordered_set<pos> grow(const std::unordered_set<pos> & curr, const std::unordered_set<pos> & holes) {
	std::unordered_set<pos> result;
	std::unordered_set<pos> adj = getAdj(curr, holes);
	if (adj.empty())
	    result = curr;
	else {
            auto cpy = curr;
            std::copy(adj.begin(), adj.end(), std::inserter(cpy, cpy.end()));
	    auto temp = grow(cpy, holes);
	    std::copy(temp.begin(), temp.end(), std::inserter(result, result.end()));
	}
	return result;
    }

    std::unordered_set<pos> getAdj(const std::unordered_set<pos> & curr, const std::unordered_set<pos> & holes) {
	std::unordered_set<pos> next;
	for (auto it = holes.begin(); it != holes.end(); ++it)
	    if (curr.find(*it) == curr.end() && isAdjTo(curr, *it))
		next.insert(*it);
	return next;
    }

    bool isAdjTo(const std::unordered_set<pos> & curr, const pos & p) {
	return std::any_of(curr.begin(), curr.end(),
			   [&](const pos & ps){
			       return p == std::make_pair(ps.first+1, ps.second) ||
				   p == std::make_pair(ps.first-1, ps.second) ||
				   p == std::make_pair(ps.first, ps.second-1) ||
				   p == std::make_pair(ps.first, ps.second+1);
			   });
    }
};


int main() {
    std::vector<std::string> board{
	"XXXX",
	    "XOOX",
	    "XXOX",
	    "XOXX"
    };
    solution soln;
    soln.solve(board);
}

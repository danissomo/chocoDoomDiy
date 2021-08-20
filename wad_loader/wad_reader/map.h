#include "../data_types.h"
#include <string>
#include <vector>
class Map{
	public:
		Map(std::string s_name);
		~Map();

		std::string get_name();
		void add_vertex(vertex &v);
		void add_linedef(linedef &l);
		vertex& get_vert(int i){
			if(i < 0 || i >= m_verexes.size() ) {
				vertex v;
				return v;
				}
			return m_verexes[i];
		}
	protected:
		std::string map_name;
		std::vector<vertex> m_verexes;
		std::vector<linedef> m_linedefs;  
};
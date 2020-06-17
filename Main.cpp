#include <iostream>
#include <cstring>
#include "lz77.hpp"
#include "Huffman.hpp"
#include "compressor.hpp"

const char test_str[] = "Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit, amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur? At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem rerum facilis est et expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat, facere possimus, omnis voluptas assumenda est, omnis dolor repellendus. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat.";

int main() {
	char* str[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
	size_t l[5];
	str[0] = new char[sizeof test_str];
	for (size_t i = 0; i < sizeof test_str; ++i) {
		str[0][i] = test_str[i];
	}
	l[0] = sizeof test_str;
	//lz77EncodeFast(str[0], l[0], str[1], l[1]);
	//huffmanEncode(str[1], l[1], str[2], l[2]);
	//huffmanDecode(str[2], l[2], str[3], l[3]);
	//lz77Decode(str[3], l[3], str[4], l[4]);
	compress_block(str[0], l[0], str[1], l[1]);
	decompress_block(str[1], l[1], str[2], l[2]);
	//std::cout << str3;
	return 0;
}
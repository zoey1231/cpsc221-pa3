
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

// Node constructor, given.
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor, given.
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


// twoDtree assignment operator, given.
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/**
    * Constructor that builds a twoDtree out of the given PNG.
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a rectangle of pixels 
    * in the rtinal PNG, represented by an (x,y) pair for the 
    * upper left corner of the rectangle and an (x,y) pair for 
    * lower right corner of the rectangle. In addition, the Node
    * stores a pixel representing the average color over the 
    * rectangle. 
    *
    * Every node's left and right children correspond to a partition
    * of the node's rectangle into two smaller rectangles. The 
    * current node's rectangle is split by a horizontal or vertical 
    * line, depending on which direction corresponds to the node's 
    * level in the tree. Even numbered levels will split vertically
    * and odd numbers will split horizontally. 
    *
    * The best splitting line in either dimension, will be the
    * one that minimizes the sum of the variances of the two 
    * resulting rectangles. (Ties will be broken by....TODO)
    * 
    * The left child of the node will contain the upper left corner
    * of the node's rectangle, and the right child will contain the
    * lower right corner.
    *
   * This function will build the stats object used to score the 
   * splitting lines. It will also call helper function buildTree.
*/
twoDtree::twoDtree(PNG & imIn){ 

	// YOUR CODE HERE
	width = imIn.width();
	height = imIn.height();
	stats s = stats(imIn);
	pair<int,int> ul = make_pair(0, 0);
	pair<int,int> lr = make_pair(width - 1, height - 1);
	bool vert = false; // since level of root is 1 (odd)

	root = buildTree(s, ul, lr, vert);
}

/**
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param s Contains the data used to split the rectangles
   * @param ul upper left point of current node's rectangle.
   * @param lr lower right point of current node's rectangle.
*/
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {

	// YOUR CODE HERE!!

	// create the current node
	/*twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)*/
	Node *curr_node = new Node(ul, lr, s.getAvg(ul, lr));

	// if the current node does not represent a single pixel
	if (ul.first != lr.first || ul.second != lr.second) {

		/*The left child of the node will contain the upper left corner
		* of the node's rectangle, and the right child will contain the
		* lower right corner.*/
		pair<int,int> left_ul = ul, left_lr, right_ul, right_lr = lr;
		int split, best_split;
		int left_sum, right_sum, score, best_score;

		// split horizontally if the given rectangle is 1 x n (ignore vert here)
		if (lr.first == ul.first) {

			// first find the best splitting line according to the sums of
			// stats scores for both split rectangles
			// reference: https://piazza.com/class/k4esgvw2pp64oh?cid=2553
			for (split = ul.second; split < lr.second; split++) {

				// make new pairs using current splitting line
				left_lr = make_pair(lr.first, split);
				right_ul = make_pair(lr.first, split + 1);

				// get score of current rectangles
				/*The best splitting line in either dimension, will be the
				* one that minimizes the sum of the variances of the two 
				* resulting rectangles. (Ties will be broken by....TODO)*/
				left_sum = s.getScore(left_ul, left_lr);
				right_sum = s.getScore(right_ul, right_lr);
				score = left_sum + right_sum;

				// compare and get best score & best splitting line
				if ((split == ul.second) || (score < best_score)) {
					best_score = score;
					best_split = split;
				}
			}

			// split rectangle using best splitting line
			left_lr = make_pair(lr.first, best_split);
			curr_node->left = buildTree(s, left_ul, left_lr, !vert);
			right_ul = make_pair(ul.first, best_split + 1);
			curr_node->right = buildTree(s, right_ul, right_lr, !vert);
		}
		
		// split vertically if the given rectangle is n x 1 (ignore vert here)
		else if (lr.second == ul.second) {

			// first find the best splitting line according to the sums of
			// stats scores for both split rectangles
			// reference: https://piazza.com/class/k4esgvw2pp64oh?cid=2553
			for (split = ul.first; split < lr.first; split++) {

				// make new pairs using current splitting line
				left_lr = make_pair(split, lr.second);
				right_ul = make_pair(split + 1, lr.second);

				// get score of current rectangles
				/*The best splitting line in either dimension, will be the
				* one that minimizes the sum of the variances of the two 
				* resulting rectangles. (Ties will be broken by....TODO)*/
				left_sum = s.getScore(left_ul, left_lr);
				right_sum = s.getScore(right_ul, right_lr);
				score = left_sum + right_sum;

				// compare and get best score & best splitting line
				if ((split == ul.first) || (score < best_score)) {
					best_score = score;
					best_split = split;
				}
			}

			// split rectangle using best splitting line
			left_lr = make_pair(best_split, lr.second);
			curr_node->left = buildTree(s, left_ul, left_lr, !vert); // since the next level must be the opposite parity
			right_ul = make_pair(best_split + 1, ul.second);
			curr_node->right = buildTree(s, right_ul, right_lr, !vert);
		}

		// split vertically according to vert
		else if (vert) {

			// first find the best splitting line according to the sums of
			// stats scores for both split rectangles
			// reference: https://piazza.com/class/k4esgvw2pp64oh?cid=2553
			for (split = ul.first; split < lr.first; split++) {

				// make new pairs using current splitting line
				left_lr = make_pair(split, lr.second);
				right_ul = make_pair(split + 1, lr.second);

				// get score of current rectangles
				/*The best splitting line in either dimension, will be the
				* one that minimizes the sum of the variances of the two 
				* resulting rectangles. (Ties will be broken by....TODO)*/
				left_sum = s.getScore(left_ul, left_lr);
				right_sum = s.getScore(right_ul, right_lr);
				score = left_sum + right_sum;

				// compare and get best score & best splitting line
				if ((split == ul.first) || (score < best_score)) {
					best_score = score;
					best_split = split;
				}
			}

			// split rectangle using best splitting line
			left_lr = make_pair(best_split, lr.second);
			curr_node->left = buildTree(s, left_ul, left_lr, !vert); // since the next level must be the opposite parity
			right_ul = make_pair(best_split + 1, ul.second);
			curr_node->right = buildTree(s, right_ul, right_lr, !vert);
		}

		// split horizontally according to vert
		else if (!vert) {

			// first find the best splitting line according to the sums of
			// stats scores for both split rectangles
			// reference: https://piazza.com/class/k4esgvw2pp64oh?cid=2553
			for (split = ul.second; split < lr.second; split++) {

				// make new pairs using current splitting line
				left_lr = make_pair(lr.first, split);
				right_ul = make_pair(lr.first, split + 1);

				// get score of current rectangles
				/*The best splitting line in either dimension, will be the
				* one that minimizes the sum of the variances of the two 
				* resulting rectangles. (Ties will be broken by....TODO)*/
				left_sum = s.getScore(left_ul, left_lr);
				right_sum = s.getScore(right_ul, right_lr);
				score = left_sum + right_sum;

				// compare and get best score & best splitting line
				if ((split == ul.second) || (score < best_score)) {
					best_score = score;
					best_split = split;
				}
			}

			// split rectangle using best splitting line
			left_lr = make_pair(lr.first, best_split);
			curr_node->left = buildTree(s, left_ul, left_lr, !vert);
			right_ul = make_pair(ul.first, best_split + 1);
			curr_node->right = buildTree(s, right_ul, right_lr, !vert);
		}
	}
	
	return curr_node;
}

/**
	* Render returns a PNG image consisting of the pixels
    * stored in the tree. It may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the 
    * average color stored in the node.
*/
PNG twoDtree::render(){

	// YOUR CODE HERE!!
	PNG image = PNG(width, height);
	recurRender(image, root); // call recursive helper function 
	return image;
}

/**
	* Private helper function for Render(). Recursively draws
   	* pixels on the PNG according to the specification of Render().
    * @param im the PNG to draw with RGB color.
    * @param rt the root of the current subtree.
*/
void twoDtree::recurRender(PNG & im, twoDtree::Node * rt) {

	// if root is a leaf node, draw avg color into the pixel
	if (rt->left == NULL && rt->right == NULL) {
		for (int w = rt->upLeft.first; w < rt->lowRight.first + 1; w++) {
			for (int h = rt->upLeft.second; h < rt->lowRight.second + 1; h++) {
				*im.getPixel(w, h) = rt->avg;
			}
		}
	}

	// if not, call recursion
	if (rt->left != NULL)
		recurRender(im, rt->left);

	if (rt->right != NULL)
		recurRender(im, rt->right);
}

/**
	 * The idealPrune function can be considered to be the inverse
	 * of the pruneSize function. It takes as input a number of leaves
	 * and returns the minimum tolerance that would produce that resolution
	 * upon a prune. It does not change the structure of the tree.
	 *    
*/
int twoDtree::idealPrune(int leaves){

	// YOUR CODE HERE!!
	int tol, min_tol = 5000; // !arbitrary

	for (tol = 5000; tol > 0; tol--) {
		if (pruneSize(tol) == leaves) {
			if (tol < min_tol)
				min_tol = tol;
		}
	}
	return min_tol;
}

/**	
    * The pruneSize function takes a tolerance as input, and returns
    * the number of leaves that would result _if_ the tree were to 
    * be pruned with that tolerance. Consistent with the definition
    * of prune, a node is counted if all of the leaves in its subtree
    * are within tol of the node's color. 
    * 
   	* You may want a recursive helper function for this one.
*/
int twoDtree::pruneSize(int tol){
    
	// YOUR CODE HERE!!
	int c = 0;
	recurPruneSize(tol, root, c, true, false); 
	return c;
}

/**
	* Private helper function for pruneSize(int tol). Recursively counts
   	* the number of leaves of the pruned tree. 
    * @param tol the tolerance of average color.
    * @param rt the root of the current subtree.
	* @param c the counter of leaves after pruning.
	* @param p the boolean to determine whether to prune the subtree.
	* @param chk the boolean to record if the node has been checked (to avoid pruning twice).
*/
void twoDtree::recurPruneSize(int tol, twoDtree::Node * rt, int & c, bool p, bool chk) {

	if (rt == NULL) return;

	// *NOTE: not actually performing pruning
	// if rt->left is a leaf node, mark rt as not to prune if its left leaf doesn't satisfy the criteria
	if (rt->left->left == NULL && rt->left->right == NULL) {
		if (!((rt->left->avg.r - rt->avg.r)^2 + (rt->left->avg.g - rt->avg.g)^2 + (rt->left->avg.b -rt->avg.b)^2 <= tol)) 
			p = false;
	} // if not, call recursion
	else recurPruneSize(tol, rt->left, c, true, false);

	// if rt->right is a leaf node, mark rt as not to prune if its right leaf doesn't satisfy the criteria
	if (rt->right->left == NULL && rt->right->right == NULL) {
		if (!((rt->right->avg.r - rt->avg.r)^2 + (rt->right->avg.g - rt->avg.g)^2 + (rt->right->avg.b -rt->avg.b)^2 <= tol))
			p = false;
	} // if not, call recursion
	else recurPruneSize(tol, rt->right, c, true, false);

	if (p && !chk) c++; // count 1 if rt is ready to be pruned (so rt is the only leaf)
	else c += 2; // count 2 if rt is not to be pruned (rtinal tree is full)
	
	chk = true;
}

/**
    *  Prune function trims subtrees as high as possible in the tree.
    *  A subtree is pruned (cleared) if all of its leaves are within 
    *  tol of the average color stored in the root of the subtree. 
    *  Pruning criteria should be evaluated on the rtinal tree, not 
    *  on a pruned subtree. (we only expect that trees would be pruned once.)
    *  
   	* You may want a recursive helper function for this one.
*/
void twoDtree::prune(int tol){

	// YOUR CODE HERE!!
	recurPrune(tol, root, true, false);
}

/**
	* Private helper function for prune(int tol). Recursively prunes
   	* the leaves that are within tol of average color stored in the given root. 
    * @param tol the tolerance of average color.
    * @param rt the root of the current subtree.
	* @param p the boolean to determine whether to prune the subtree.
	* @param chk the boolean to record if the node has been checked (to avoid pruning twice).
*/
void twoDtree::recurPrune(int tol, twoDtree::Node * rt, bool p, bool chk) {

	if (rt == NULL) return;

	// reference: https://piazza.com/class/k4esgvw2pp64oh?cid=2887
	// if rt->left is a leaf node, mark rt as not to prune if its left leaf doesn't satisfy the criteria
	if (rt->left->left == NULL && rt->left->right == NULL) {

		if (!((rt->left->avg.r - rt->avg.r)^2 + (rt->left->avg.g - rt->avg.g)^2 + (rt->left->avg.b -rt->avg.b)^2 <= tol)) 
			p = false;
	} // if not, call recursion
	else recurPrune(tol, rt->left, true, false);

	// if rt->right is a leaf node, mark rt as not to prune if its right leaf doesn't satisfy the criteria
	if (rt->right->left == NULL && rt->right->right == NULL) {
		if (!((rt->right->avg.r - rt->avg.r)^2 + (rt->right->avg.g - rt->avg.g)^2 + (rt->right->avg.b -rt->avg.b)^2 <= tol))
			p = false;
	} // if not, call recursion
	else recurPrune(tol, rt->right, true, false);

	// *NOTE: the tree is a full tree before pruning
	// prune the subtree only if p == true and the subtree is not yet pruned
	if (p && !chk) {
		delete(rt->left);
		rt->left = NULL;
		delete(rt->right);
		rt->right = NULL;
	}
	chk = true;
}

/**
	 * Destroys all dynamically allocated memory associated with the
	 * current twoDtree class. Complete for PA3.
	 * You may want a recursive helper function for this one.
*/
void twoDtree::clear() {

	// YOUR CODE HERE!!
	recurClear(root);
}

/**
	* Private helper function for clear(). Recursively clears
   	* the nodes created in twoDtree class.
    * @param rt the root of the current subtree.
*/
void twoDtree::recurClear(twoDtree::Node * rt) {

	if (rt == NULL) return;  
  
    recurClear(rt->left);  
    recurClear(rt->right);  
       
    delete(rt);
	rt = NULL;
}

/**
	 * Copies the parameter other twoDtree into the current twoDtree.
	 * Does not free any memory. Called by copy constructor and op=.
	 * You may want a recursive helper function for this one.
	 * @param other The twoDtree to be copied.
 */
void twoDtree::copy(const twoDtree & other) {

	// YOUR CODE HERE!!
	width = other.width;
	height = other.height;
	root = recurCopy(other.root);
}

/**
	* Private helper function for copy(). Recursively copys
   	* the nodes in other twoDtree into the current twoDtree.
	* @param rt The current node to be copied.
*/
twoDtree::Node * twoDtree::recurCopy(twoDtree::Node * rt) {

	if (rt == NULL) return NULL;

 	Node *tmp = new Node(rt->upLeft, rt->lowRight, rt->avg);
	tmp->left = recurCopy(rt->left);
	tmp->right = recurCopy(rt->right);
	
	return tmp;
}
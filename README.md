# cpsc221-pa3
https://www.students.cs.ubc.ca/~cs-221/2019W2/mps/p3/
 
#to-do lists:
    In class stats:

        long getSum(char channel, pair<int,int> ul, pair<int,int> lr)
        long getSumSq(char channel, pair<int,int> ul, pair<int,int> lr)
        stats(PNG & im)
        long getScore(pair<int,int> ul, pair<int,int> lr)
        RGBAPixel getAvg(pair<int,int> ul, pair<int,int> lr)
        long rectArea(pair<int,int> ul, pair<int,int> lr)

    In class twoDtree:

        void clear()
        void copy(const twoDtree & other)
        twoDtree(PNG & imIn)
        PNG render()
        void prune(int tol)
        int pruneSize(int tol)
        int idealPrune(int leaves)
        Node * buildTree(stats & s,pair<int,int> ul, pair<int,int> lr, bool vert)
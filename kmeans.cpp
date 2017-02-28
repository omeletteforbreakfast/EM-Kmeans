#include <iostream>  
#include <fstream>  
#include <vector>  
#include <math.h>  
#define k 8  
using namespace std;
//存放元组的属性信息  
struct Tuple {
	float attr1;
	float attr2;
	float attr3;
	float attr4;
	float attr5;
	float attr6;
	float attr7;
	float attr8;
};
//计算两个元组间的欧几里距离  
float getDistXY(Tuple t1, Tuple t2)
{
	return sqrt((t1.attr1 - t2.attr1) * (t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2) * (t1.attr2 - t2.attr2) + (t1.attr3 - t2.attr3) * (t1.attr3 - t2.attr3) + (t1.attr4 - t2.attr4) * (t1.attr4 - t2.attr4) + (t1.attr5 - t2.attr5) * (t1.attr5 - t2.attr5) + (t1.attr6 - t2.attr6) * (t1.attr6 - t2.attr6) + (t1.attr7 - t2.attr7) * (t1.attr7 - t2.attr7) + (t1.attr8 - t2.attr8) * (t1.attr8 - t2.attr8));
}

//根据质心，决定当前元组属于哪个簇  
int clusterOfTuple(Tuple means[], Tuple tuple) {
	float dist = getDistXY(means[0], tuple);
	float tmp;
	int label = 0;//标示属于哪一个簇  
	for (int i = 1; i<k; i++) {
		tmp = getDistXY(means[i], tuple);
		if (tmp<dist) { dist = tmp; label = i; }
	}
	return label;
}
//获得给定簇集的平方误差  
float getVar(vector<Tuple> clusters[], Tuple means[]) {
	float var = 0;
	for (int i = 0; i < k; i++)
	{
		vector<Tuple> t = clusters[i];
		for (int j = 0; j< t.size(); j++)
		{
			var += getDistXY(t[j], means[i]);
		}
	}
	//cout<<"sum:"<<sum<<endl;  
	return var;

}
//获得当前簇的均值（质心）  
Tuple getMeans(vector<Tuple> cluster) {

	int num = cluster.size();
	//此处means1，means2...means8代表八个维度而非八个点
	double means1 = 0, means2 = 0, means3 = 0, means4 = 0, means5 = 0, means6 = 0, means7 = 0, means8 = 0;
	Tuple t;
	for (int i = 0; i < num; i++)
	{
		means1 += cluster[i].attr1;
		means2 += cluster[i].attr2;
		means3 += cluster[i].attr3;
		means4 += cluster[i].attr4;
		means5 += cluster[i].attr5;
		means6 += cluster[i].attr6;
		means7 += cluster[i].attr7;
		means8 += cluster[i].attr8;
	}
	t.attr1 = means1 / num;
	t.attr2 = means2 / num;
	t.attr3 = means3 / num;
	t.attr4 = means4 / num;
	t.attr5 = means5 / num;
	t.attr6 = means6 / num;
	t.attr7 = means7 / num;
	t.attr8 = means8 / num;
	return t;
	//cout<<"sum:"<<sum<<endl;  


}
void KMeans(vector<Tuple> tuples) {
	ofstream outfile("output.txt");
	vector<Tuple> clusters[k];
	Tuple means[k];
	int i = 0;
	//默认一开始将前K个元组的值作为k个簇的质心（均值）  
	for (i = 0; i<k; i++) {
		means[i].attr1 = tuples[i].attr1;
		means[i].attr2 = tuples[i].attr2;
		means[i].attr3 = tuples[i].attr3;
		means[i].attr4 = tuples[i].attr4;
		means[i].attr5 = tuples[i].attr5;
		means[i].attr6 = tuples[i].attr6;
		means[i].attr7 = tuples[i].attr7;
		means[i].attr8 = tuples[i].attr8;
	}
	int lable = 0;
	//根据默认的质心给簇赋值  
	for (i = 0; i != tuples.size(); ++i) {
		lable = clusterOfTuple(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	//输出刚开始的簇  
	for (lable = 0; lable<k; lable++) {
		//cout << "第" << lable + 1 << "个簇：" << endl;
		vector<Tuple> t = clusters[lable];
		/*for (i = 0; i< t.size(); i++)
		{
		cout << "(" << t[i].attr1 << "," << t[i].attr2 << "," << t[i].attr3 << "," << t[i].attr4 << "," << t[i].attr5 << "," << t[i].attr6 << "," << t[i].attr7 << "," << t[i].attr8 << ")" << "   ";
		}
		cout << endl;*/
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  
	{

		for (i = 0; i < k; i++) //更新每个簇的中心点  
		{
			means[i] = getMeans(clusters[i]);
			//cout<<"means["<<i<<"]:"<<means[i].attr1<<"  "<<means[i].attr2<<endl;  
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值  
		for (i = 0; i < k; i++) //清空每个簇  
		{
			clusters[i].clear();
		}
		//根据新的质心获得新的簇  
		for (i = 0; i != tuples.size(); ++i) {
			lable = clusterOfTuple(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}
	for (lable = 0; lable<k; lable++) {
		outfile << "第" << lable + 1 << "个簇：" << endl;
		vector<Tuple> t = clusters[lable];
		for (i = 0; i< t.size(); i++)
		{
			outfile << "(" << t[i].attr1 << "," << t[i].attr2 << "," << t[i].attr3 << "," << t[i].attr4 << "," << t[i].attr5 << "," << t[i].attr6 << "," << t[i].attr7 << "," << t[i].attr8 << ")" << "   ";
		}
		outfile << endl;
		outfile << "中心点为" << "(" << means[lable].attr1 << "," << means[lable].attr2 << "," << means[lable].attr3 << "," << means[lable].attr4 << "," << means[lable].attr5 << "," << means[lable].attr6 << "," << means[lable].attr7 << "," << means[lable].attr8 << ")" << "   ";
		outfile << endl;
	}
	outfile.close();

}
int main() {

	char fname[256];
	cout << "请输入存放数据的文件名： ";
	cin >> fname;
	cout << endl;
	ifstream infile;
	infile.open(fname, ios::in);
	if (!infile) {
		cout << "不能打开输入的文件" << fname << endl;
		return 0;
	}
	int count = 0;
	vector<Tuple> tuples;
	Tuple tuple;
	//从文件流中读入数据  
	while (!infile.eof()) {
		count++;
		if (count % 8 == 1) infile >> tuple.attr1;
		if (count % 8 == 2) infile >> tuple.attr2;
		if (count % 8 == 3) infile >> tuple.attr3;
		if (count % 8 == 4) infile >> tuple.attr4;
		if (count % 8 == 5) infile >> tuple.attr5;
		if (count % 8 == 6) infile >> tuple.attr6;
		if (count % 8 == 7) infile >> tuple.attr7;
		if (count % 8 == 0) { infile >> tuple.attr8; tuples.push_back(tuple); }
	}
	//int k;  
	//cout<<"请输入期望的簇的个数："  
	//cin>>k;  
	//cout<<endl;  
	//输出文件中的元组信息  
	/*for (vector<Tuple>::size_type ix = 0; ix != tuples.size(); ++ix)
	cout << "(" << tuples[ix].attr1 << "," << tuples[ix].attr2 << "," << tuples[ix].attr3 << "," << tuples[ix].attr4 << "," << tuples[ix].attr5 << "," << tuples[ix].attr6 << "," << tuples[ix].attr7 << "," << tuples[ix].attr8 << ")" << "    ";
	cout << endl;*/
	KMeans(tuples);
	return 0;
}

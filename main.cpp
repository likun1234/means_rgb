#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
using namespace cv;
using namespace std;

/*
   此程序计算所有图片每个通道的均值（b,g,r）

 */

//减少颜色空间
void colorReduce8(cv::Mat &image, int div=64) {
	cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();
	for ( ; it!= itend; ++it) {
		(*it)[0]= (*it)[0]/div*div + div/2;
		(*it)[1]= (*it)[1]/div*div + div/2;
		(*it)[2]= (*it)[2]/div*div + div/2;
	}
}


void colorReduce(cv::Mat& img, int div)
{
	int n = cv::log(static_cast<double>(div)) / cv::log(2.0);
	int mask = 0xFF << n;

	// 方式一：
	//cv::Mat_<cv::Vec3b>::iterator it = img.begin<cv::Vec3b>();
	// 方式二：
	cv::MatIterator_<cv::Vec3b> it = img.begin<cv::Vec3b>();
	for(; it!=img.end<cv::Vec3b>(); it++)
	{
		(*it)[0] = (*it)[0] & mask + div / 2;
		(*it)[1] = (*it)[1] & mask + div / 2;
		(*it)[2] = (*it)[2] & mask + div / 2;
	}
}


void colorReduce0(cv::Mat &image, int div=64) {
	int nr= image.rows; // number of rows
	int nc= image.cols * image.channels(); // total number of elements per line
	for (int j=0; j<nr; j++) {
		uchar* data= image.ptr<uchar>(j);
		for (int i=0; i<nc; i++) {
			data[i]= data[i]/div*div + div/2;
		}
	}
}




Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));
	const int channels = I.channels();
	switch(channels)
	{
		case 1:
			{
				MatIterator_<uchar> it, end;
				for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
					*it = table[*it];
				break;
			}
		case 3:
			{
				MatIterator_<Vec3b> it, end;
				for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
				{
					(*it)[0] = table[(*it)[0]];
					(*it)[1] = table[(*it)[1]];
					(*it)[2] = table[(*it)[2]];
				}
			}
	}
	return I;
}

void output(Mat &mat)
{
	int temp_a=0,temp_b=0,temp_c=0;
	int index=0;
	int img_cols = mat.cols;
	int img_rows = mat.rows;
	for (int i=0;i<img_cols;i++){
		for(int j=0;j<img_rows;j++){
			index = j*img_rows + i;
			cout<<mat.at<Vec3b>(j,i)<<endl;
			temp_a = mat.at<Vec3b>(j,i)[0];
			temp_b = mat.at<Vec3b>(j,i)[1];
			temp_c = mat.at<Vec3b>(j,i)[2];
			cout<<":  "<<temp_a<<"   "<<temp_b<<"   "<<temp_c<<endl;
			//num++;
		}
	}

	//cout<<num<<endl;
	cout<<img_cols<<"   "<<img_rows<<endl;
}


int main(){
	char imgposition[100];
	Mat img,mat;
	int img_cols;
	int img_rows;
	int temp_a=0,temp_b=0,temp_c=0;
	int index=0;
	double num=0,num_p=0;
	MatIterator_<Vec3b> it, end;//定义迭代器
	Scalar kk(255,255,255);
	double b_sum_o = 0,g_sum_o = 0,r_sum_o = 0;
	double b_sum_av = 0,g_sum_av = 0,r_sum_av = 0;
	double b_sum = 0,g_sum = 0,r_sum = 0;
	double b_sum_means = 0,g_sum_means = 0,r_sum_means = 0;
	for(int i=51;i<201;i++){
		if(i<100){
			sprintf(imgposition,"/home/lk/deep_learning/FCN-2018-5-09/image/0%d.jpg",i);
		}
		else{
			sprintf(imgposition,"/home/lk/deep_learning/FCN-2018-5-09/image/%d.jpg",i);
		}
		mat = imread(imgposition,1);
		if(mat.empty()){
			//cout<<"img is empty "<<endl;
			continue;
			//return -1;
		}
		cout<<i<<endl;
		//cv::imshow("1",mat);
		//waitKey(0);

		b_sum_o = 0;
		g_sum_o = 0;
		r_sum_o = 0;
		for(it=mat.begin<Vec3b>(),end=mat.end<Vec3b>();it!=end;it++){

			temp_a = (*it)[0];//b
			b_sum_o +=temp_a;
			temp_b = (*it)[1];//g
			g_sum_o +=temp_b;
			temp_c = (*it)[2];//r
			r_sum_o +=temp_c;
			//num++;

			//cout<<"(*it) is :  "<<temp_a<<"   "<<temp_b<<"   "<<temp_c<<endl;
		}
		img_cols = mat.cols;
		img_rows = mat.rows;
		b_sum_av = b_sum_o/(img_cols*img_rows);//每张图片的r,g,b均值
		g_sum_av = g_sum_o/(img_cols*img_rows);
		r_sum_av = r_sum_o/(img_cols*img_rows);

		b_sum +=b_sum_av;
		g_sum +=g_sum_av;
		r_sum +=r_sum_av;
		num_p++;
	}
	cout<<"num_p is "<<num_p<<endl;
	b_sum_means = b_sum/num_p;
	g_sum_means = g_sum/num_p;
	r_sum_means = r_sum/num_p;

	cout<<img_cols<<"  "<<img_rows<<endl;
	//cout<<num<<endl;
	cout<<b_sum_means<<"   "<<g_sum_means<<"    "<<r_sum_means<<endl;//输出所有图片的均值结果
	return 1;
}

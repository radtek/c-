// ViewDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "ViewDialog.h"
#include "afxdialogex.h"


// CViewDialog 对话框

IMPLEMENT_DYNAMIC(CViewDialog, CDialogEx)

CViewDialog::CViewDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewDialog::IDD, pParent)
{

}

CViewDialog::~CViewDialog()
{
}

void CViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDialog, CDialogEx)
END_MESSAGE_MAP()


// CViewDialog 消息处理程序



//************************************
// 函数名称:    ConvertMat2CImage
// 访问权限:    public 
// 创建日期:    2016/10/26
// 创 建 人:	    
// 函数说明:    将cv::Mat类型的图像数据转换为CImage类型的数据
// 函数参数:    const cv::Mat & src_img 输入的cv::Mat图像数据
// 函数参数:    CImage & dst_img			转换之后的CImage类型
// 返 回 值:    BOOL
//************************************
BOOL ConvertMat2CImage(const cv::Mat& src_img, CImage& dst_img)
{
	if (!src_img.data)
	{
		return FALSE;
	}
	int width = src_img.cols;			//获取输入图像的宽度
	int height = src_img.rows;			//获取输入图像的高度
	int channels = src_img.channels();	//获取输入图像的
	int src_type = src_img.type();
 
	dst_img.Destroy();
 
	switch (src_type)
	{
	case CV_8UC1:
	{
		dst_img.Create(width, -1*height, 8 * channels);
		unsigned char* dst_data = static_cast<unsigned char*>(dst_img.GetBits());
		int step_size = dst_img.GetPitch();		//获取位图行与行之间相差的字节数
		unsigned char* src_data = nullptr;
		for (int i = 0; i < height; i++)
		{
			src_data = const_cast<unsigned char*>(src_img.ptr<unsigned char>(i));	//获取行指针
			for (int j = 0; j < width; j++)
			{
				if (step_size > 0)
				{
					*(dst_data + step_size*i + j) = *src_data++;
				}	//像素的排列方式是自左上开始的
				else
				{
					*(dst_data + step_size*i - j) = *src_data++;
				}
			}
		}
		break;
	}
	case CV_8UC3:
	{
		dst_img.Create(width, height, 8 * channels);
		unsigned char* dst_data = static_cast<unsigned char*>(dst_img.GetBits());
		int step_size = dst_img.GetPitch();		//获取位图行与行之间相差的字节数
		unsigned char* src_data = nullptr;
		for (int i = 0; i < height; i++)
		{
			src_data = const_cast<unsigned char*>(src_img.ptr<unsigned char>(i));	//获取行指针
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					*(dst_data + step_size*i + j * 3 + k) = src_data[3 * j + k];
				}
			}
		}
		break;
	}
	default:

		break;
	}
 
	return TRUE;
}


BOOL GetFixMat(const cv::Mat& src_img, cv::Mat& dst_img, unsigned int dc_heigh, unsigned int dc_width)
{
	if (!src_img.data)
	{
		return FALSE;
	}
	unsigned int img_rows(src_img.rows);
	unsigned int img_cols(src_img.cols);
	unsigned int fix_heigh(std::min(img_rows, dc_heigh));
	unsigned int fix_width(std::min(img_cols, dc_width));

	float ratio_w = static_cast<float>(fix_width) / static_cast<float>(img_cols);
	float ratio_h = static_cast<float>(fix_heigh) / static_cast<float>(img_rows);
	float ratio = std::min(ratio_w, ratio_h);

	int show_width = static_cast<unsigned int>(ratio * img_cols);
	int show_height = static_cast<unsigned int>(ratio * img_rows);

	cv::resize(src_img, dst_img, cv::Size(show_width, show_height), (0.0), (0.0), cv::INTER_LINEAR);

	return TRUE;
}


void CViewDialog::showCamera(cv::Mat img)
{
	GetDlgItem(IDD_VIEW_DIALOG);
	/*
	//改变图片大小适应picture控件
	CRect pic_rect;
	int width, height;
	GetDlgItem(IDC_STATIC_CAMERA)->GetClientRect(&pic_rect);
	width = pic_rect.right;
	height = pic_rect.bottom;
	cv::Mat showImg;
	resize(img, showImg, Size(width, height));
	imshow("disparity", showImg);
	*/
	/*
	CLogger::Instance()->TraceInfo("showCamera: start");
	if (!img.data)
	{
		CLogger::Instance()->TraceInfo("showCamera: !img.data");
		return;
	}
	
	//控件大小和位置
	CRect rectCtrl;
	CLogger::Instance()->TraceInfo("showCamera: 1");
	CWnd *pDlgItem = (CWnd*)GetDlgItem(IDC_STATIC_CAMERA);
	CLogger::Instance()->TraceInfo("showCamera: 2");
	pDlgItem->MoveWindow(100,100,100,100);//更改控件大小并移动其到指定位置
	CLogger::Instance()->TraceInfo("showCamera: 3");
	pDlgItem->GetWindowRect(rectCtrl);
	CLogger::Instance()->TraceInfo("showCamera: 4");
	this->ScreenToClient(rectCtrl);
	CLogger::Instance()->TraceInfo("showCamera: 5");

	unsigned int dc_heigh = rectCtrl.Width();
	unsigned int dc_width = rectCtrl.Height();
	CLogger::Instance()->TraceInfo("showCamera: 6");
	CImage dst_img;
	cv::Mat temp;	//定义中间变量
	GetFixMat(img, temp, dc_heigh, dc_width);	//图像的几何大小变换
	CLogger::Instance()->TraceInfo("showCamera: 7");
	ConvertMat2CImage(temp, dst_img);			//图像转换
	CLogger::Instance()->TraceInfo("showCamera: 8");
	int offsetx = (dc_width - temp.cols) / 2;			//调整偏移量
	int offsety = (dc_heigh - temp.rows) / 2;
	dst_img.Draw(pDlgItem->GetDC()->GetSafeHdc(), offsetx, offsety, dst_img.GetWidth(), dst_img.GetHeight());	//图像显示
	CLogger::Instance()->TraceInfo("showCamera: 9");
	*/
}

BOOL CViewDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	
	namedWindow("disparity", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("disparity");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_CAMERA)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// ViewDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "ViewDialog.h"
#include "afxdialogex.h"


// CViewDialog �Ի���

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


// CViewDialog ��Ϣ�������



//************************************
// ��������:    ConvertMat2CImage
// ����Ȩ��:    public 
// ��������:    2016/10/26
// �� �� ��:	    
// ����˵��:    ��cv::Mat���͵�ͼ������ת��ΪCImage���͵�����
// ��������:    const cv::Mat & src_img �����cv::Matͼ������
// ��������:    CImage & dst_img			ת��֮���CImage����
// �� �� ֵ:    BOOL
//************************************
BOOL ConvertMat2CImage(const cv::Mat& src_img, CImage& dst_img)
{
	if (!src_img.data)
	{
		return FALSE;
	}
	int width = src_img.cols;			//��ȡ����ͼ��Ŀ��
	int height = src_img.rows;			//��ȡ����ͼ��ĸ߶�
	int channels = src_img.channels();	//��ȡ����ͼ���
	int src_type = src_img.type();
 
	dst_img.Destroy();
 
	switch (src_type)
	{
	case CV_8UC1:
	{
		dst_img.Create(width, -1*height, 8 * channels);
		unsigned char* dst_data = static_cast<unsigned char*>(dst_img.GetBits());
		int step_size = dst_img.GetPitch();		//��ȡλͼ������֮�������ֽ���
		unsigned char* src_data = nullptr;
		for (int i = 0; i < height; i++)
		{
			src_data = const_cast<unsigned char*>(src_img.ptr<unsigned char>(i));	//��ȡ��ָ��
			for (int j = 0; j < width; j++)
			{
				if (step_size > 0)
				{
					*(dst_data + step_size*i + j) = *src_data++;
				}	//���ص����з�ʽ�������Ͽ�ʼ��
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
		int step_size = dst_img.GetPitch();		//��ȡλͼ������֮�������ֽ���
		unsigned char* src_data = nullptr;
		for (int i = 0; i < height; i++)
		{
			src_data = const_cast<unsigned char*>(src_img.ptr<unsigned char>(i));	//��ȡ��ָ��
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
	//�ı�ͼƬ��С��Ӧpicture�ؼ�
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
	
	//�ؼ���С��λ��
	CRect rectCtrl;
	CLogger::Instance()->TraceInfo("showCamera: 1");
	CWnd *pDlgItem = (CWnd*)GetDlgItem(IDC_STATIC_CAMERA);
	CLogger::Instance()->TraceInfo("showCamera: 2");
	pDlgItem->MoveWindow(100,100,100,100);//���Ŀؼ���С���ƶ��䵽ָ��λ��
	CLogger::Instance()->TraceInfo("showCamera: 3");
	pDlgItem->GetWindowRect(rectCtrl);
	CLogger::Instance()->TraceInfo("showCamera: 4");
	this->ScreenToClient(rectCtrl);
	CLogger::Instance()->TraceInfo("showCamera: 5");

	unsigned int dc_heigh = rectCtrl.Width();
	unsigned int dc_width = rectCtrl.Height();
	CLogger::Instance()->TraceInfo("showCamera: 6");
	CImage dst_img;
	cv::Mat temp;	//�����м����
	GetFixMat(img, temp, dc_heigh, dc_width);	//ͼ��ļ��δ�С�任
	CLogger::Instance()->TraceInfo("showCamera: 7");
	ConvertMat2CImage(temp, dst_img);			//ͼ��ת��
	CLogger::Instance()->TraceInfo("showCamera: 8");
	int offsetx = (dc_width - temp.cols) / 2;			//����ƫ����
	int offsety = (dc_heigh - temp.rows) / 2;
	dst_img.Draw(pDlgItem->GetDC()->GetSafeHdc(), offsetx, offsety, dst_img.GetWidth(), dst_img.GetHeight());	//ͼ����ʾ
	CLogger::Instance()->TraceInfo("showCamera: 9");
	*/
}

BOOL CViewDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	
	namedWindow("disparity", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("disparity");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_CAMERA)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

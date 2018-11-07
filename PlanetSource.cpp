#include"DxLib.h"
#include<stdio.h>
#define _USE_MATH_DEFINES//M_PI�p
#include<math.h>

#define e 0.0934//�O�����S��(�ΐ�)
#define ee 0.01671022//�O�����S��(�n��)
#define T 687//���]����(�ΐ�)
#define TT 365.25636300//���]����(�n��)
#define a 1.52368//�O�������a(�ΐ�)
#define aa 1.0000//�O�������a(�n��)
#define b a * sqrt(1 - e * e)//�O���Z���a(�ΐ�)
#define bb aa*sqrt(1-ee*ee)//�O���Z���a(�n��)
#define ms 49.512*M_PI/180//����_���o(�ΐ�)
#define es 174.836*M_PI/180//����_���o(�n��)
#define mk 1.849*M_PI/180//�O���X�Ίp(�ΐ�)
#define ek 0.002*M_PI/180//�O���X�Ίp(�n��)
#define mi 286.64*M_PI/180//�ߓ��_����(�ΐ�)
#define ei (-71.829)*M_PI/180//�ߓ��_����(�n��)

//�����E�X���擾
double getJulian(int nYear, int nMonth, double dDay)
{
	double da, db;
	double dJD;

	if (nMonth < 3) {
		nMonth += 12;
		nYear--;
	}

	// Calc. body
	da = floor(nYear / 100.0);
	db = 2.0 - da + floor(da / 4.0);
	dJD = floor(365.25 * nYear) + floor(30.6001 * (nMonth + 1)) + dDay + db + 1720994.5;

	if (dJD < 2299160.5) {
		dJD -= db;
	}

	return dJD;
}
//�����E�X��������t�擾
void juian_date(double dJD, int *nYear, int *nMonth, double *dDay)
{
	int nY, nM;
	double da, db, dd;
	double dJDs = dJD + 0.5;
	double dJDi = floor(dJDs);
	double dJDf = dJDs - dJDi;

	if (dJDi > 2299160.0) {
		da = floor((dJDi - 1867216.25) / 36524.25);
		dJDi += (1.0 + da - floor(da / 4.0));
	}

	db = dJDi + 1524.0;
	nY = (int)(floor((db - 122.1) / 365.25));
	dd = db - floor(nY * 365.25);
	nM = (int)(floor(dd / 30.6001));

	// Write Day
	*dDay = dd - floor(nM * 30.6001) + dJDf;

	// Write Month
	if (nM < 14) {
		*nMonth = nM - 1;
	}
	else {
		*nMonth = nM - 13;
	}

	// Write Year
	if (*nMonth > 2) {
		*nYear = nY - 4716;
	}
	else {
		*nYear = nY - 4715;
	}
}
//F(u)��u-e*sin(u)-n(t-t0)
double fnf(double u, double year, double month, double day) {
	return u - e * sin(u) - (2 * M_PI / T)*(getJulian(year, month, day) - getJulian(2016, 10, 29));
}
//u_n+1=u_n-F(u)/F'(u)
double fng(double u) {
	return 1 - e * cos(u);
}
//�n��ver
double fnfe(double u, double year, double month, double day) {
	return u - ee * sin(u) - (2 * M_PI / TT)*(getJulian(year, month, day) - getJulian(2016, 1, 3));
}
//�n��ver
double fnge(double u) {
	return 1 - ee * cos(u);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(FALSE);//windows�T�C�Y�ݒ�
	if (DxLib_Init() == -1) {//�������ݒ�
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);//����ʏ���

	int color = GetColor(255, 255, 255);//�����F(��)
	int colortwo = GetColor(0, 0, 0);//�����F(��)
	double aaa;//n�N��܂Ōv�Z�p
	double u, un, julian;//u,un�͌v�Z�p�Ajulian�̓����E�X���\���p
	double uu, unn;
	int year = 2013, month = 7, day = 31;//�N����
	int count = 0;//60����1�b�ҋ@�p
	int Handle;//�摜�\���p
	int Handle1;//�n��
	int Handle2;//���z
	double mxx, myy;//�񎟌��ł̍��W(�ΐ�)
	double mxxx, myyy;//�O�����ł̍��W(�ΐ�)
	double exx, eyy;//�񎟌��ł̍��W(�n��)
	double exxx, eyyy;//�O�����ł̍��W(�n��)
	double mx[10000];//x���W����p(�ΐ�)
	double my[10000];//y���W����p(�ΐ�)
	double ex[10000];//x���W����p(�n��)
	double ey[10000];//y���W����p(�n��)
	int hoge = 0;//���ɖ߂��p
	int kaisu=0;//�e�v�f�ɍ��W�����p

	//Handle�ɉ摜��n��
	Handle = LoadGraph("�摜/mars.jpg");//�ΐ�
	Handle1 = LoadGraph("�摜/ontheearth.jpg");//�L
	Handle2 = LoadGraph("�摜/sun.png");//���z

	//�\���p�ϐ�julian�Ƀ����E�X�������
	julian = getJulian(year, month, day);

	//5�N��܂Ōv�Z�p����
	aaa = julian;

	// while(����ʂ�\��ʂɔ��f, ���b�Z�[�W����, ��ʃN���A)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		u = (2 * M_PI / T)*(getJulian(year, month, day) - getJulian(2016, 10, 29));
		uu = (2 * M_PI / TT)*(getJulian(year, month, day) - getJulian(2016, 1, 3));

		//count��+1
		count++;

		//60����1�b�o�Ǝ��s��
		if (count == 1) {
			//��ʂ����œh��Ԃ�
			DrawBox(0, 0, 640, 480, colortwo, FALSE);
			//����\��
			DrawFormatString(0, 0, color, "����:%d", year);
			//���\��
			DrawFormatString(0, 20, color, "��:%d", month);
			//���\��
			DrawFormatString(0, 40, color, "��:%d", day);
			//�����E�X���\��
			DrawFormatString(0, 60, color, "�����E�X��:%lf", julian);

			//�j���[�g���@
			//�ΐ�ver
			while (1) {
				un = u - fnf(u, year, month, day) / fng(u);
				if (fabs(un - u) < 0.000000001) break;
				u = un;
			}
			//�n��ver
			while (1) {
				unn = uu - fnfe(uu, year, month, day) / fnge(uu);
				if (fabs(unn - uu) < 0.000000001) break;
				uu = unn;
			}

			//x����y�����v�Z
			mxx = a * cos(u) - a * e;
			myy = b * sin(u);
			exx = aa * cos(uu) - aa * ee;
			eyy = bb * sin(uu);

			//�O�����ł̍��W�v�Z
			mxxx = mxx * (cos(ms)*cos(mi) - sin(ms)*cos(mk)*sin(mi)) - myy * (cos(ms)*sin(mi) + sin(ms)*cos(mk)*cos(mi));
			myyy = mxx * (sin(ms)*cos(mi) + cos(ms)*cos(mk)*sin(mi)) - myy * (sin(ms)*sin(mi) - cos(ms)*cos(mk)*cos(mi));
			exxx = exx * (cos(es)*cos(ei) - sin(es)*cos(ek)*sin(ei)) - eyy * (cos(es)*sin(ei) + sin(es)*cos(ek)*cos(ei));
			eyyy = exx * (sin(es)*cos(ei) + cos(es)*cos(ek)*sin(ei)) - eyy * (sin(es)*sin(ei) - cos(es)*cos(ek)*cos(ei));

			//���ꂼ��̍��W�\��
			DrawFormatString(0, 80, color, "�ΐ�");
			DrawFormatString(0, 100, color, "x: %f", mxxx);
			DrawFormatString(0, 120, color, "y: %f", myyy);
			DrawFormatString(0, 140, color, "�n��");
			DrawFormatString(0, 160, color, "x: %f", exxx);
			DrawFormatString(0, 180, color, "y: %f", eyyy);

			//�_�`�恫
			//�v�f�ɍ��W����
			mx[kaisu] = mxxx;
			my[kaisu] = myyy;
			ex[kaisu] = exxx;
			ey[kaisu] = eyyy;
			//��U�a����
			hoge = kaisu;
			//�O���`��
			for (kaisu = 0; kaisu < hoge + 1; kaisu++) {
				DrawPixel(400 + (120 * mx[kaisu]), 250 - (120 * my[kaisu]), color);
				DrawPixel(400 + (120 * ex[kaisu]), 250 - (120 * ey[kaisu]), color);
			}
			//kaisu�����̒l�ɂ�����
			kaisu = hoge;

			//�ΐ��O���`��
			DrawRotaGraph(400, 250, 0.3, 0.0, Handle2, TRUE);
			DrawRotaGraph(400 + (120 * mxxx), 250 - (120 * myyy), 0.3, 0.0, Handle, FALSE);
			DrawRotaGraph(400 + (120 * exxx), 250 - (120 * eyyy), 0.3, 0.0, Handle1, FALSE);

			//�����E�X��+1
			julian = julian + 1;
			//�ܔN�o�ƏI���
			if (julian == (aaa + 1827)) {
				break;
			}
			//���t�X�V��
			//2����28���܂�
			if (month == 2) {
				if (day == 28) {
					day = 0;
					month = month + 1;
				}
			}
			//��������
			if (month == 4 || month == 6 || month == 9 || month == 11) {
				if (day == 30) {
					day = 0;
					month = month + 1;
				}
			}
			//���������ȊO
			else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
				if (day == 31) {
					day = 0;
					month = month + 1;
				}
			}
			//12���𒴂����year�𑝂₵�Amonth��1�ɂ���
			if (month == 13) {
				month = 1;
				year = year + 1;
			}
			//���t+1
			day = day + 1;
			//�ēx60����1�b�҂���
			count = 0;
			//�v�f����̂���+1
			kaisu = kaisu + 1;
		}
	}
	WaitKey();//�L�[���͂�����܂ŕ\��
	DxLib_End();//DX���C�u�����I������
	return 0;
}
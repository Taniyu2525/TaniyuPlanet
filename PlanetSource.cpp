#include"DxLib.h"
#include<stdio.h>
#define _USE_MATH_DEFINES//M_PI用
#include<math.h>

#define e 0.0934//軌道離心率(火星)
#define ee 0.01671022//軌道離心率(地球)
#define T 687//公転周期(火星)
#define TT 365.25636300//公転周期(地球)
#define a 1.52368//軌道長半径(火星)
#define aa 1.0000//軌道長半径(地球)
#define b a * sqrt(1 - e * e)//軌道短半径(火星)
#define bb aa*sqrt(1-ee*ee)//軌道短半径(地球)
#define ms 49.512*M_PI/180//昇交点黄経(火星)
#define es 174.836*M_PI/180//昇交点黄経(地球)
#define mk 1.849*M_PI/180//軌道傾斜角(火星)
#define ek 0.002*M_PI/180//軌道傾斜角(地球)
#define mi 286.64*M_PI/180//近日点引数(火星)
#define ei (-71.829)*M_PI/180//近日点引数(地球)

//ユリウス日取得
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
//ユリウス日から日付取得
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
//F(u)≡u-e*sin(u)-n(t-t0)
double fnf(double u, double year, double month, double day) {
	return u - e * sin(u) - (2 * M_PI / T)*(getJulian(year, month, day) - getJulian(2016, 10, 29));
}
//u_n+1=u_n-F(u)/F'(u)
double fng(double u) {
	return 1 - e * cos(u);
}
//地球ver
double fnfe(double u, double year, double month, double day) {
	return u - ee * sin(u) - (2 * M_PI / TT)*(getJulian(year, month, day) - getJulian(2016, 1, 3));
}
//地球ver
double fnge(double u) {
	return 1 - ee * cos(u);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(FALSE);//windowsサイズ設定
	if (DxLib_Init() == -1) {//初期化設定
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);//裏画面処理

	int color = GetColor(255, 255, 255);//文字色(白)
	int colortwo = GetColor(0, 0, 0);//文字色(黒)
	double aaa;//n年後まで計算用
	double u, un, julian;//u,unは計算用、julianはユリウス日表示用
	double uu, unn;
	int year = 2013, month = 7, day = 31;//年月日
	int count = 0;//60分の1秒待機用
	int Handle;//画像表示用
	int Handle1;//地球
	int Handle2;//太陽
	double mxx, myy;//二次元での座標(火星)
	double mxxx, myyy;//三次元での座標(火星)
	double exx, eyy;//二次元での座標(地球)
	double exxx, eyyy;//三次元での座標(地球)
	double mx[10000];//x座標代入用(火星)
	double my[10000];//y座標代入用(火星)
	double ex[10000];//x座標代入用(地球)
	double ey[10000];//y座標代入用(地球)
	int hoge = 0;//元に戻す用
	int kaisu=0;//各要素に座標入れる用

	//Handleに画像を渡す
	Handle = LoadGraph("画像/mars.jpg");//火星
	Handle1 = LoadGraph("画像/ontheearth.jpg");//猫
	Handle2 = LoadGraph("画像/sun.png");//太陽

	//表示用変数julianにユリウス日入れる
	julian = getJulian(year, month, day);

	//5年後まで計算用処理
	aaa = julian;

	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		u = (2 * M_PI / T)*(getJulian(year, month, day) - getJulian(2016, 10, 29));
		uu = (2 * M_PI / TT)*(getJulian(year, month, day) - getJulian(2016, 1, 3));

		//countを+1
		count++;

		//60分の1秒経つと実行↓
		if (count == 1) {
			//画面を黒で塗りつぶす
			DrawBox(0, 0, 640, 480, colortwo, FALSE);
			//西暦表示
			DrawFormatString(0, 0, color, "西暦:%d", year);
			//月表示
			DrawFormatString(0, 20, color, "月:%d", month);
			//日表示
			DrawFormatString(0, 40, color, "日:%d", day);
			//ユリウス日表示
			DrawFormatString(0, 60, color, "ユリウス日:%lf", julian);

			//ニュートン法
			//火星ver
			while (1) {
				un = u - fnf(u, year, month, day) / fng(u);
				if (fabs(un - u) < 0.000000001) break;
				u = un;
			}
			//地球ver
			while (1) {
				unn = uu - fnfe(uu, year, month, day) / fnge(uu);
				if (fabs(unn - uu) < 0.000000001) break;
				uu = unn;
			}

			//x軸とy軸を計算
			mxx = a * cos(u) - a * e;
			myy = b * sin(u);
			exx = aa * cos(uu) - aa * ee;
			eyy = bb * sin(uu);

			//三次元での座標計算
			mxxx = mxx * (cos(ms)*cos(mi) - sin(ms)*cos(mk)*sin(mi)) - myy * (cos(ms)*sin(mi) + sin(ms)*cos(mk)*cos(mi));
			myyy = mxx * (sin(ms)*cos(mi) + cos(ms)*cos(mk)*sin(mi)) - myy * (sin(ms)*sin(mi) - cos(ms)*cos(mk)*cos(mi));
			exxx = exx * (cos(es)*cos(ei) - sin(es)*cos(ek)*sin(ei)) - eyy * (cos(es)*sin(ei) + sin(es)*cos(ek)*cos(ei));
			eyyy = exx * (sin(es)*cos(ei) + cos(es)*cos(ek)*sin(ei)) - eyy * (sin(es)*sin(ei) - cos(es)*cos(ek)*cos(ei));

			//それぞれの座標表示
			DrawFormatString(0, 80, color, "火星");
			DrawFormatString(0, 100, color, "x: %f", mxxx);
			DrawFormatString(0, 120, color, "y: %f", myyy);
			DrawFormatString(0, 140, color, "地球");
			DrawFormatString(0, 160, color, "x: %f", exxx);
			DrawFormatString(0, 180, color, "y: %f", eyyy);

			//点描画↓
			//要素に座標を代入
			mx[kaisu] = mxxx;
			my[kaisu] = myyy;
			ex[kaisu] = exxx;
			ey[kaisu] = eyyy;
			//一旦預ける
			hoge = kaisu;
			//軌道描画
			for (kaisu = 0; kaisu < hoge + 1; kaisu++) {
				DrawPixel(400 + (120 * mx[kaisu]), 250 - (120 * my[kaisu]), color);
				DrawPixel(400 + (120 * ex[kaisu]), 250 - (120 * ey[kaisu]), color);
			}
			//kaisuを元の値にかえす
			kaisu = hoge;

			//火星軌道描画
			DrawRotaGraph(400, 250, 0.3, 0.0, Handle2, TRUE);
			DrawRotaGraph(400 + (120 * mxxx), 250 - (120 * myyy), 0.3, 0.0, Handle, FALSE);
			DrawRotaGraph(400 + (120 * exxx), 250 - (120 * eyyy), 0.3, 0.0, Handle1, FALSE);

			//ユリウス日+1
			julian = julian + 1;
			//五年経つと終わり
			if (julian == (aaa + 1827)) {
				break;
			}
			//日付更新↓
			//2月は28日まで
			if (month == 2) {
				if (day == 28) {
					day = 0;
					month = month + 1;
				}
			}
			//西向く侍
			if (month == 4 || month == 6 || month == 9 || month == 11) {
				if (day == 30) {
					day = 0;
					month = month + 1;
				}
			}
			//西向く侍以外
			else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
				if (day == 31) {
					day = 0;
					month = month + 1;
				}
			}
			//12月を超えるとyearを増やし、monthを1にする
			if (month == 13) {
				month = 1;
				year = year + 1;
			}
			//日付+1
			day = day + 1;
			//再度60分の1秒待つため
			count = 0;
			//要素代入のため+1
			kaisu = kaisu + 1;
		}
	}
	WaitKey();//キー入力があるまで表示
	DxLib_End();//DXライブラリ終了処理
	return 0;
}
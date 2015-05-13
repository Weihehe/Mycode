package com.Cartoonifier;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.util.AttributeSet;
import android.view.View;


public class LView extends View {
 private Paint paint;  
  public LView(Context context) {
  super(context);
 } 
  public LView(Context context, AttributeSet attrs) {
  super(context, attrs);
 } 
  @Override
 protected void onDraw(Canvas canvas) {
  super.onDraw(canvas);
  paint = new Paint();// newһ������
  paint.setColor(Color.RED);// ���û�����ɫ
  paint.setStyle(Style.FILL);// ���û������
  canvas.drawCircle(50, 50, 40, paint);// �û����ڻ���������һ��Բ����ֻ��������Բ�����������Ӿ��εȣ�
  paint.setColor(Color.YELLOW);// ���û�����ɫ
  canvas.drawText("LView", 50, 50, paint);// �û����ڻ������������֣��м�����������Ӧ�������ꡣ
 }
}
 
package com.Cartoonifier;

import java.io.ObjectInputStream.GetField;

import android.content.Context;
import android.graphics.Bitmap;

public class CartoonifierView {
	
	
	private boolean bSaveThisFrame = false;
	private boolean bFreezeOutput = false;
	private static final int FREEZE_OUTPUT_MSECS = 3000;
	private int[] mRGBA;
	public CartoonifierView(Context context) {
        
    }

	protected void nextFrameShouldBeSaved(Context baseContext) {
		// TODO Auto-generated method stub
		bSaveThisFrame = true;
	}
	//��ͨͼ���Ԥ��ͼ��֮����л�
	protected Bitmap processFrame(byte[]data) {
		int[] rgba = mRGBA;
		if(bFreezeOutput)
		{
			bFreezeOutput = false;
			try
			{
				wait(FREEZE_OUTPUT_MSECS);
			}catch(InterruptedException e)
			{
				e.printStackTrace();
			}
			return null;
		}
		if(!bSaveThisFrame)
		{
			ShowPreview(width, height, yuv, rgba));
		}
		
	}
	
	//C++ native ��������
	public native void ShowPreview(int width, int height, byte[] yuv, int[] rgba);
    public native void CartoonifyImage(int width, int height, byte[] yuv, int[] rgba, boolean sketchMode, boolean alienMode, boolean evilMode, boolean debugMode);
    
    //���ڶ���
    static {
        System.loadLibrary("cartoonifier");
    }
}
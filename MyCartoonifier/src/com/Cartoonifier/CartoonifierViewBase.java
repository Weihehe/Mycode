package com.Cartoonifier;

import java.io.IOException;
import java.util.List;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.os.Build;

import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public abstract class CartoonifierViewBase extends SurfaceView implements SurfaceHolder.Callback, Runnable {

	private static final String TAG = "Cartoonifier::SurfaceView";
	
	private Camera mCamera;
	private SurfaceHolder mHolder;
	private int mFrameWidth;
	private int mFrameHeight;
	private byte[] mFrame;
	private boolean mThreadRun;
	private byte[] mBuffer;
	
	//相机已经初始化好
	private boolean mCameraIsInitialized = false;
	
	public CartoonifierViewBase(Context context) {
		super(context); 
		// 通过SurfaceView获得SurfaceHolder对象  
		mHolder = getHolder();
		// 为holder添加回调结构SurfaceHolder.Callback  
		mHolder.addCallback(this);
		// TODO Auto-generated constructor stub
	}
	public int getFrameWidth()
	{
		return mFrameWidth;
	}
	public int getFrameHeight()
	{
		return mFrameHeight;	
	}
	public void setPreview()throws IOException
	{
		if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.HONEYCOMB)
			mCamera.setPreviewTexture(new SurfaceTexture(10));
		else
			mCamera.setPreviewDisplay(null);
	}
	//打开相机
	public boolean openCamera()
	{
		Log.i(TAG, "openCamera");
		releaseCamera();
		mCamera = Camera.open();
		if(mCamera == null)
		{
			Log.e(TAG, "Can't open camera!");
			return false;
		}
		mCamera.setPreviewCallbackWithBuffer(new PreviewCallback() {
			
			@Override
			public void onPreviewFrame(byte[] data, Camera camera) {
				// TODO Auto-generated method stub
				System.arraycopy(data, 0, mFrame, 0, data.length);
				camera.addCallbackBuffer(mBuffer);
				
				mCameraIsInitialized = true;
			}
		});
		return true;
	}
	//释放相机
	public void releaseCamera()
	{
		Log.i(TAG, "releaseCamera");
		mThreadRun = false;
		//给对象和方法或者代码块加锁
		synchronized (this) {
			if(mCamera!=null)
			{
				mCamera.stopPreview();
				mCamera.setPreviewCallback(null);
				mCamera.release();
				mCamera = null;
				mCameraIsInitialized = false;
			}
			
		}
		onPreviewStopped();
	}
	//摄像机的设置工作
	public void setupCamera(int width,int height)
	{
		Log.i(TAG, "setupCamera(" + width + "x" + height + ")");
		synchronized (this) {
			if(mCamera!=null)
			{
				Camera.Parameters params = mCamera.getParameters();
				List<Camera.Size> sizes = params.getSupportedPreviewSizes();
				mFrameWidth = width;
				mFrameHeight = height;
				
				
				//选择合适的并且最大的屏幕
					int minDiff = Integer.MAX_VALUE;
					for(Camera.Size size:sizes)
					{
						Log.i(TAG, "Found Camera Resolution " + size.width + "x" + size.height);
						if(Math.abs(size.height-size.width)<minDiff)
						{
							mFrameWidth = size.width;
							mFrameHeight = size.height;
							minDiff = Math.abs(size.height-height);
						}
					}
					
				
				//设置摄像机的长宽
				params.setPreviewSize(getFrameWidth(), getFrameHeight());
				
				//摄像机支持的模式
				List<String> FocusModes = params.getSupportedFocusModes();
				if(FocusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO))
				{
					params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
				}
				mCamera.setParameters(params);
				
				//分配缓冲区
				params = mCamera.getParameters();
				Log.i(TAG, "Chosen Camera Preview Size: " + params.getPreviewSize().width + "x" + params.getPreviewSize().height);
				int size = params.getPreviewSize().width*params.getPreviewSize().height;
				size = size *ImageFormat.getBitsPerPixel(params.getPreviewFormat())/8;
				mBuffer = new byte[size];
				mFrame = new byte[size];
				//设置回调缓冲区
				mCamera.addCallbackBuffer(mBuffer);	
				
				try 
				{
					setPreview();
				}catch(IOException e)
				{
					Log.e(TAG, "mCamera.setPreviewDisplay/setPreviewTexture fails: " + e);
				}
				
				
				
			}
		}
	}
	
	protected abstract void onPreviewStarted(int previewWidtd, int previewHeight);
	
	protected abstract void onPreviewStopped();
}

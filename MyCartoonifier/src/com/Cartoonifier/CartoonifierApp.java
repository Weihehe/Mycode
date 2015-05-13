package com.Cartoonifier;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;


public class CartoonifierApp extends Activity implements OnTouchListener{
	
	 private static final String TAG = "CartoonifierApp";
	 private CartoonifierView mView;

	 

	 private Spinner spinner2;
	 private ArrayAdapter adapter2;
	 private TextView view2;
	 private Button Photobutton;
	 
	 public CartoonifierApp() {
	        Log.i(TAG, "Instantiated new " + this.getClass());
	    }
	 protected void onPause() {
	        Log.i(TAG, "onPause");
	        super.onPause();
	        mView.releaseCamera();
	 }
	    protected void onResume() {
	        Log.i(TAG, "onResume");
	        super.onResume();
	        if( !mView.openCamera() ) {
	            AlertDialog ad = new AlertDialog.Builder(this).create();  
	            ad.setCancelable(false); // This blocks the 'BACK' button  
	            ad.setMessage("Fatal error: can't open camera!");  
	            ad.setButton("OK", new DialogInterface.OnClickListener() {  
	                public void onClick(DialogInterface dialog, int which) {  
	                    dialog.dismiss();                      
	                    finish();
	                }  
	            });  
	            ad.show();
	        }
	    }
	 
	 public void onCreate(Bundle savedInstanceState) {

	        super.onCreate(savedInstanceState);
	        requestWindowFeature(Window.FEATURE_NO_TITLE);
	   //     mView = new CartoonifierView(this);

	        
	        setContentView(R.layout.camera_main);
	        mView = (CartoonifierView) findViewById(R.id.MyCartoonifierView); ;
	        // Call our "onTouch()" callback function whenever the user touches the screen.
	     //   mView.setOnTouchListener(this);
	      Photobutton = (Button) findViewById(R.id.phptobutton);

	      Photobutton.setOnClickListener(new View.OnClickListener() {

	      public void onClick(View v)
	      {
	    	  mView.nextFrameShouldBeSaved(getBaseContext());
	      }
	      });
	        
	        spinner2 = (Spinner) findViewById(R.id.spinner_mode);
	        view2 = (TextView) findViewById(R.id.textView1);
	 
	        //将可选内容与ArrayAdapter连接起来
	        adapter2 = ArrayAdapter.createFromResource(this, R.array.camera_mode, android.R.layout.simple_spinner_item);
	 
	        //设置下拉列表的风格 
	        adapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	 
	        //将adapter2 添加到spinner中
	        spinner2.setAdapter(adapter2);
	 
	        //添加事件Spinner事件监听  
	        spinner2.setOnItemSelectedListener(new SpinnerXMLSelectedListener());
	 
	        //设置默认值
	        spinner2.setVisibility(View.VISIBLE);
	        
	    }
	public boolean onTouch(View v, MotionEvent m) {
		if(m.getAction()!=MotionEvent.ACTION_DOWN)
		{
			return false;
		}
		mView.nextFrameShouldBeSaved(getBaseContext());
		return true;
	}
	
	
	//使用XML形式操作
    class SpinnerXMLSelectedListener implements OnItemSelectedListener{
        public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,
                long arg3) {
            view2.setText("照相机的模式是："+adapter2.getItem(arg2));
            Log.i(TAG, "Camera Mode selected: " + adapter2.getItem(arg2));
            if(adapter2.getItem(arg2).equals("正常相机"))
            {
            	mView.toggleNormalMode();
            }
            else if(adapter2.getItem(arg2).equals("卡通"))
            {
            	mView.toggleCartoonMode();
            }
            else if(adapter2.getItem(arg2).equals("素描"))
            {
            	mView.toggleSketchMode();
            }
            else if(adapter2.getItem(arg2).equals("怪物"))
            {
            	mView.toggleEvilMode();
            }
            else if(adapter2.getItem(arg2).equals("外星人"))
            {
            	mView.toggleAlienMode();
            	view2.setText("照相机的模式是："+adapter2.getItem(arg2)+"(将脸放在人脸轮廓中)");
            }
            else if(adapter2.getItem(arg2).equals("艺术"))
            {
            	mView.toggleArtMode();
            }
            else if(adapter2.getItem(arg2).equals("模糊"))
            {
            	mView.toggleBlurMode();
            }
        }
 
        public void onNothingSelected(AdapterView<?> arg0) {
             
        }
         
    }
	

}

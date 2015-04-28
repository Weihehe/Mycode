package com.Cartoonifier;

import android.app.Activity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;


public class CartoonifierApp extends Activity implements OnTouchListener{

	 private CartoonifierView mView;

	 public void onCreate(Bundle savedInstanceState) {

	        super.onCreate(savedInstanceState);

	        mView = new CartoonifierView(this);
	        setContentView(mView);

	        // Call our "onTouch()" callback function whenever the user touches the screen.
	        mView.setOnTouchListener(this);
	    }
	public boolean onTouch(View v, MotionEvent m) {
		if(m.getAction()!=MotionEvent.ACTION_DOWN)
		{
			return false;
		}
		mView.nextFrameShouldBeSaved(getBaseContext());
		return true;
	}

}

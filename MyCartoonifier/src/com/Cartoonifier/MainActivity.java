package com.Cartoonifier;

import android.app.Activity;
import android.content.Intent;

import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import android.widget.ImageView;


public class MainActivity extends Activity {

	private ImageView mTouchIn;  
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        
        mTouchIn = (ImageView) findViewById(R.id.touch); 
        
        
    //    Animation alphaAnimation = AnimationUtils.loadAnimation(this, R.anim.push_in); 
   //     alphaAnimation.setRepeatCount(10);
  //      mLoading.startAnimation(alphaAnimation); 
   /*     AlphaAnimation animation = new AlphaAnimation(0,1); 
        animation.setDuration(2000);
        mLoading.setAnimation(animation); 
        animation.startNow(); 
        animation.reset();*/
        
      //��˸  
        AlphaAnimation alphaAnimation1 = new AlphaAnimation(0.0f, 1.0f);  
        alphaAnimation1.setDuration(1000);  
        alphaAnimation1.setRepeatCount(Animation.INFINITE);  
        alphaAnimation1.setRepeatMode(Animation.REVERSE);  
        mTouchIn.setAnimation(alphaAnimation1);  
        alphaAnimation1.start(); 
        
        mTouchIn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Intent intent = new Intent(MainActivity.this,CartoonifierApp.class);
				startActivity(intent);
			}
		});
        
    }
}
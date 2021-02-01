package com.example.traingledemo

import android.content.Context
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(MyGLSurface(applicationContext))
    }
}

class MyGLSurface(context: Context?) : GLSurfaceView(context) {
    init {
        setEGLContextClientVersion(2)
        setRenderer(MyRenderer())
    }
}


class MyRenderer : GLSurfaceView.Renderer {






    override fun onSurfaceCreated(gl: GL10, config: EGLConfig?) {
        Log.d("xwc","1")
        NativeInterFace.initOpenGL()
    }

    override fun onDrawFrame(gl: GL10) {
        Log.d("xwc","2")
        NativeInterFace.renderOneFrame()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        Log.d("xwc","3")
        NativeInterFace.onViewportChange(width.toFloat(), height.toFloat())
    }

}

class NativeInterFace {
    companion object {
        external fun renderOneFrame()
        external fun onViewportChange(width: Float, height: Float)
        external fun initOpenGL()
    }
}

package com.mex.jagent;

import java.lang.instrument.Instrumentation;

public class Premain {
    public static void main(String[] args) {
        System.out.println("hello premain");
    }

    public static void premain(String arg, Instrumentation instrumentation) {
        System.out.println("hello premain in premain");
    }
}

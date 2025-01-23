// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

package mdi;

public class Manga {
    public static void main(String[] args) {
        String storeName = (args.length > 0) ? args[0] : "Untitled";
        Controller controller = new Controller(storeName);
        controller.mdi();
    }
}

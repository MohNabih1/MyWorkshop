// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package mdi;

import store.Store;

public enum View {
    CUSTOMERS, ORDERS, PRODUCTS;
    private static String[] views = new String[]{"Customers", "Orders", "Products"};
    @Override
    public String toString() {
        return views[this.ordinal()];
    }
}

// Or, using a custom constructor...
/*

public enum View {
    TOOLS("Tools", PLANTS("Plants"), CUSTOMERS("Customers");
    private String string;
    private View(String string) {
        this.string = string;
    }
    @Override
    public String toString() {
        return string;
    }
}

*/

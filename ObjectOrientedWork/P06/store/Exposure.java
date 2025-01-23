// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package store;

public enum Exposure {
    SHADE, PARTSUN, SUN;
    private static final String[] text = new String[]{
        "Full Shade", "Part Sun/Shade", "Full Sun"
    };
    @Override
    public String toString() {
        return text[this.ordinal()];
    }
}

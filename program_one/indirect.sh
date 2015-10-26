#!/bin/bash

x=2
letters=(a b c d)

function fn() {

    echo ${!1}
}

fn x
fn letters
#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as py
import pandas as pd

############# 

#How can I make a function of out this? 
#parameters to be passed in are: path, 

#assumptions: 
#*All files located in the specified 
#*First row from each raw data file is deleted 
#*tab separated txt files are being imported


#############

#1. Look for all txt files in specific directory
import glob
path = r'C:\Users\Charles Avila\Desktop\Data Files\OASPL_data' #specify local path
all_files = glob.glob(path + "/*.txt") #list of file names

li = [] #empty list

for filename in all_files:
    df = pd.read_csv(filename, sep='\t') #tab separated data
    li.append(df) #list of data frames
    #print(f"list item:\n {li}")

frame = pd.concat(li, axis=1, ignore_index=False) #create large data frame

print(f"Large data from all Data Files \n {frame}") #print frame including X [s]

#2. time axis selection
time_axis = frame.iloc[:,0] #select all rows, but only first (0) column
print(f"Time Axis \n {time_axis}")

#3. Drop all X [s] axis columns
no_x_df = frame.drop('X [s]',axis=1) #remove all X [s]
print(f"No x data frame: \n {no_x_df}")

#4. append time axis column to frame without X [s]
data_frame = pd.concat([time_axis,no_x_df],axis=1)
print(f"New data frame \n {data_frame}")


# In[2]:


time_axis = frame.iloc[:,0] #select all rows, but only first (0) column
print(f"Time Axis \n {time_axis}")


# In[3]:


#2. Drop all X [s] axis columns
no_x_df = frame.drop('X [s]',axis=1) #remove all X [s]
print(f"No x data frame: \n {no_x_df}")


# In[4]:


#3. append time axis column to frame without X [s]
data_frame = pd.concat([time_axis,no_x_df],axis=1)
print(f"New data frame \n {data_frame}")


# In[5]:


#4. Drop time-axis for averaging
dropped_frame = data_frame.drop('X [s]', axis=1)
dropped_frame


# In[6]:


#5. Average of data across rows
# not including time axis
# what's header info if put in text file? - only '0' is applied as column/header info
average = dropped_frame.mean(axis=1)
print(average)
average.to_csv('average_data.txt',sep='\t',header=['Average_OASPL'],index = False) #index = False, strips the column indices


# In[7]:


small_fry = data_frame.iloc[:,0:4] #integer location of colums 0 to 3 and all rows (:)
small_fry


# In[17]:


#average function - I'll need to specify what columns to average across depending on header names
#take average across rows 
small_fry.mean(axis=1)
#need to add column names to this


# In[18]:


(list(frame.columns.values)) 


# In[19]:


(list(small_fry.columns.values)) 


# In[20]:


max_value = small_fry.max()
max_value #return a series with column names as indices


# In[21]:


#max_value function

#find index of max value of every column
maxValueIndex = small_fry.idxmax(axis = 0) 
maxValueIndex
#use this to determine how much shift to apply to a particular data set
#based on trigger input
# I should really graph this first


# In[ ]:


small_fry.iloc[171,:] #value of data frame at a given row,column ID


# In[ ]:





# In[ ]:





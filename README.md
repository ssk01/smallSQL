# smallSQL
a small sql
## create table
create table(
    age int unique;
    name char(16);
    height float;
);
## unique 
## create index
index 有一个feature必须是unique的。没有B tree,是拿std::map写的，懒得换成multi。
## insert
有unique 时候要检查是否重复，在没有index的情况下，会爆炸慢。
## drop table
## drop index 
## delete record
## buffer
有lru的算法
牺牲时如果是dirty就会回写。
## select
如果有index时搜索，显然是快很多的。
插 100， 500， 1000条记录，select速度。
